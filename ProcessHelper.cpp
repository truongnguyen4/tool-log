#include "ProcessHelper.hpp"
#include <QProcess>
#include "Logger.hpp"
#include "mainwindow.h"
#include <QThread>
#include "NotificationHelper.hpp"

QString const ProcessHelper::TAG = "ProcessHelper";
QProcess *ProcessHelper::processReadLogCat = new QProcess();

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

int ProcessHelper::clearLogcat(const QString deviceId)
{
    if (deviceId.isEmpty())
    {
        Logger::d(TAG, "Can't clear log due to DEVICE ID is empty.");
        return MainWindow::ERROR_DEVICE_ID;
    }
    Logger::d(TAG, "Clear logcat on DEVICE ID = " + deviceId);
    QStringList command = {"-s", deviceId, "logcat", "-c"};
    int exitCode = 0;
    runShellCommand("adb", command, exitCode);
    if (exitCode != 0)
    {
        NotificationHelper::showExitCode();
        return MainWindow::ERROR_DEVICE_ID;
    }
    return MainWindow::SUCCESS;
}

int ProcessHelper::startWatchLog(const QString filePath, const QString deviceId)
{
    // Check if the file is already being watched, stop the current watch
    if (processReadLogCat->state() == QProcess::Running)
    {
        processReadLogCat->kill();
        processReadLogCat->waitForFinished();
        Logger::d(TAG, "Stop watching log");
        return MainWindow::SUCCESS;
    }

    if (deviceId.isEmpty())
    {
        Logger::d(TAG, "Can't start watching device due to DEVICE ID is empty.");
        return MainWindow::ERROR_DEVICE_ID;
    }

    if (!FileHelper::checkPath(filePath))
    {
        Logger::d(TAG, "Can't start watching log due to FILE PATH is invalid.");
        return MainWindow::ERROR_FILE_PATH;
    }

    ProcessHelper::processReadLogCat->setStandardOutputFile(filePath, QIODevice::NewOnly);
    ProcessHelper::processReadLogCat->start("adb", QStringList() << "-s" << deviceId << "logcat");
    Logger::d(TAG, "Start watching log, PID = " + QString::number(processReadLogCat->processId()) + ", DEVICE ID = " + deviceId);

    return MainWindow::SUCCESS;
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
}

void ProcessHelper::stop()
{
    if (mThreadDetectDevices != nullptr)
    {
        Logger::d(TAG, "Stop thread detect devices");
        mThreadDetectDevices->quit();
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
