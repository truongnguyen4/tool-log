#include "ProcessHelper.hpp"
#include <QProcess>
#include "Logger.hpp"
#include "mainwindow.h"
#include <QThread>
#include "NotificationHelper.hpp"

QString const ProcessHelper::TAG = "ProcessHelper";

void ProcessHelper::registerDeviceChangeListener(DeviceChangeListener *listener)
{
    if (listener == nullptr)
    {
        return;
    }
    if (!mDeviceChangeListeners.contains(listener))
    {
        Logger::d(TAG, "Register new device change listener");
        mDeviceChangeListeners.append(listener);

        QMutexLocker locker(&mtxCurrentDeviceIds);
        mCurrentDeviceIds = getDeviceIds();
        listener->onDevicesIsConnected(mCurrentDeviceIds); // Initial call to provide current devices
        locker.unlock();
    }
}

void ProcessHelper::unregisterDeviceChangeListener(DeviceChangeListener *listener)
{
    Logger::d(TAG, "Unregister device change listener");
    mDeviceChangeListeners.removeAll(listener);
}

QString ProcessHelper::runShellCommand(const QString program, const QStringList args, int &exitCode)
{
    QProcess *process = new QProcess();
    process->start(program, args);
    process->waitForFinished();

    QString output = process->readAllStandardOutput();
    exitCode = process->exitCode();

    delete process;

    return output;
}

QStringList ProcessHelper::getDeviceIds()
{
    QStringList deviceIds;
    int exitCode = 0;
    QString output = runShellCommand("adb", QStringList() << "devices", exitCode);
    if (exitCode != 0)
    {
        NotificationHelper::showExitCode();
    }
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    if (lines.length() > 1)
    {
        for (int i = 1; i < lines.length(); i++)
        {
            const QString deviceId = lines[i].split('\t')[0].trimmed();
            if (!deviceId.isEmpty())
            {
                deviceIds.append(deviceId);
            }
        }
    }
    return deviceIds;
    // return MainWindow::simulateDevices;
}

void ProcessHelper::clearLogcat(const QString deviceId)
{
    if (deviceId.isEmpty())
    {
        Logger::w(TAG, "Can't clear log due to DEVICE ID is empty.");
        return;
    }
    Logger::d(TAG, "Clear logcat on DEVICE ID = " + deviceId);
    QStringList command = {"-s", deviceId, "logcat", "-c"};
    int exitCode = 0;
    runShellCommand("adb", command, exitCode);
    if (exitCode != 0)
    {
        NotificationHelper::showExitCode();
        return;
    }
}

void ProcessHelper::startWatchLog(const QString filePath, const QString deviceId)
{
    // Check if the file is already being watched, stop the current watch
    if (processReadLogCat->state() == QProcess::Running)
    {
        processReadLogCat->kill();
        processReadLogCat->waitForFinished();
        Logger::d(TAG, "Stop watching log");
        return;
    }

    if (deviceId.isEmpty())
    {
        Logger::d(TAG, "Can't start watching device due to DEVICE ID is empty.");
        return;
    }

    if (!FileHelper::checkPath(filePath))
    {
        Logger::d(TAG, "Can't start watching log due to FILE PATH is invalid.");
        return;
    }

    ProcessHelper::processReadLogCat->setStandardOutputFile(filePath, QIODevice::NewOnly);
    ProcessHelper::processReadLogCat->start("adb", QStringList() << "-s" << deviceId << "logcat");
    Logger::d(TAG, "Start watching log, PID = " + QString::number(processReadLogCat->processId()) + ", DEVICE ID = " + deviceId);
}

ProcessHelper::ProcessHelper()
{
    mThreadDetectDevices = QThread::create([=]()
                                           { detectDevices(); });

    if (mThreadDetectDevices != nullptr)
    {
        Logger::d(TAG, "Start thread detect devices");
        mThreadDetectDevices->start();
    }
    mProcessRealtimeLog->setProcessChannelMode(QProcess::MergedChannels);
    auto buffer = std::make_shared<QByteArray>();
    QObject::connect(mProcessRealtimeLog, &QProcess::readyReadStandardOutput, mProcessRealtimeLog, [this, buffer]()
                     {
                         buffer->append(mProcessRealtimeLog->readAllStandardOutput());
                         int idx = buffer->indexOf('\n');
                         if (idx == -1)
                         {
                             return;
                         }
                         QByteArray raw = buffer->left(idx);
                         buffer->remove(0, idx + 1);

                         QString line = QString::fromUtf8(raw).trimmed();
                         Log log = LogHelper::convertToLog(line);
                         LogHelper::getInstance()->mListLogs.append(log);
                         LogHelper::getInstance()->updateHiddenLog(log);
                         insertLogToTable(log); });
}

void ProcessHelper::stop()
{
    if (mThreadDetectDevices != nullptr)
    {
        Logger::d(TAG, "Stop thread detect devices");
        mThreadDetectDevices->quit();
    }
    if (processReadLogCat != nullptr && processReadLogCat->state() == QProcess::Running)
    {
        Logger::d(TAG, "Stop process read logcat");
        processReadLogCat->kill();
        processReadLogCat->waitForFinished();
    }
    if (mProcessRealtimeLog != nullptr && mProcessRealtimeLog->state() == QProcess::Running)
    {
        Logger::d(TAG, "Stop process read logcat real-time");
        mProcessRealtimeLog->kill();
        mProcessRealtimeLog->waitForFinished();
    }
}

void ProcessHelper::detectDevices()
{
    Logger::d(TAG, "Start detecting devices");
    mCurrentDeviceIds = getDeviceIds();

    while (true)
    {
        QThread::sleep(1); // delay 1 seconds
        QStringList newDeviceIds = getDeviceIds();
        if (newDeviceIds == mCurrentDeviceIds)
        {
            continue;
        }

        QStringList addedDevices;
        for (const QString &device : newDeviceIds)
        {
            if (!mCurrentDeviceIds.contains(device))
            {
                addedDevices.append(device);
            }
        }
        if (!addedDevices.isEmpty())
        {
            Logger::d(TAG, "Device connected: " + addedDevices.join(", "));
            for (DeviceChangeListener *listener : mDeviceChangeListeners)
            {
                listener->onDevicesIsConnected(addedDevices);
            }
        }

        QStringList removedDevices;
        for (const QString &device : mCurrentDeviceIds)
        {
            if (!newDeviceIds.contains(device))
            {
                removedDevices.append(device);
            }
        }
        if (!removedDevices.isEmpty())
        {
            Logger::d(TAG, "Device disconnected: " + removedDevices.join(", "));
            for (DeviceChangeListener *listener : mDeviceChangeListeners)
            {
                listener->onDevicesIsDisconnected(removedDevices);
            }
        }
        Logger::d(TAG, "Update mCurrentDeviceIds");
        mCurrentDeviceIds = newDeviceIds;
    }
}

void ProcessHelper::startWatchLogRealTime(const QString deviceId)
{
    if (deviceId.isEmpty())
    {
        Logger::e(TAG, "Invalid parameters");
        return;
    }
    if (mProcessRealtimeLog->state() == QProcess::Running)
    {
        Logger::w(TAG, "Stopping previous real-time log process");
        mProcessRealtimeLog->kill();
        mProcessRealtimeLog->waitForFinished();
        return;
    }

    Logger::d(TAG, "Starting real-time log for device ID: " + deviceId);
    QStringList command = {"-s", deviceId, "logcat"};
    mProcessRealtimeLog->start("adb", command);
}

void ProcessHelper::insertLogToTable(Log log)
{
    LogHelper::getInstance()->updateHiddenLog(log);
    UiHandler::getInstance()->insertLogToTable(log);
}
