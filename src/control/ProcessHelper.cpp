#include "ProcessHelper.hpp"
#include <QProcess>
#include "Logger.hpp"
#include <QThread>
#include "NotificationHelper.hpp"
#include "FileHelper.hpp"
#include "UiHandler.hpp"

void ProcessHelper::registerDeviceChangeListener(DeviceChangeListener *listener)
{
    if (listener == nullptr)
    {
        Logger::w(TAG, "Trying to register a null device change listener");
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

QString ProcessHelper::runShellCommand(const QString program, const QStringList args)
{
    QProcess *process = new QProcess();
    process->start(program, args);
    process->waitForFinished();

    QString output = process->readAllStandardOutput();
    if (process->exitCode() != 0)
    {
        Logger::e(TAG, "Command failed: " + program + " " + args.join(" ") + "\nError: " + process->readAllStandardError());
        NotificationHelper::showError(NotificationHelper::ERROR_PROCESS_SHELL);
    }
    delete process;

    return output;
}

QStringList ProcessHelper::getDeviceIds()
{
    QStringList deviceIds;
    QString output = runShellCommand("adb", QStringList() << "devices");
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
    runShellCommand("adb", command);
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

    if (deviceId.isEmpty() || !FileHelper::getInstance()->checkPath(filePath))
    {
        Logger::w(TAG, "Invalid device ID or file path");
        return;
    }

    ProcessHelper::processReadLogCat->setStandardOutputFile(filePath, QIODevice::NewOnly);
    ProcessHelper::processReadLogCat->start("adb", QStringList() << "-s" << deviceId << "logcat");
    Logger::d(TAG, "Start watching log, PID = " + QString::number(processReadLogCat->processId()) + ", DEVICE ID = " + deviceId);
}

ProcessHelper::ProcessHelper()
{
    mTimerUpdateTable->setInterval(1000); // 1 second
    mTimerUpdateTable->setSingleShot(false);
   

    Logger::d(TAG, "Start thread detect devices");
    mThreadDetectDevices->start();

    Logger::d(TAG, "Start thread receive real-time log");
    mThreadRealTimeLog->start();

    // Insert log to table every second
    QObject::connect(mTimerUpdateTable, &QTimer::timeout, mTimerUpdateTable, [this]()
                     {
        if (ProcessHelper::mLastLogId < mLogHelper->mListObjs.size())
        {
            UiHandler::getInstance()->insertLogToTable(mLogHelper->mListObjs.mid(ProcessHelper::mLastLogId));
            ProcessHelper::mLastLogId = mLogHelper->mListObjs.size();
        } });

    // Read real-time log output
    QObject::connect(mProcessRealtimeLog, &QProcess::readyReadStandardOutput, mProcessRealtimeLog, [this]()
                     { mBufferLogs->append(mProcessRealtimeLog->readAllStandardOutput()); });
}

// Read real-time log from mBufferLogs: used in thread mThreadRealTimeLog
void ProcessHelper::receiveRealTimeLog()
{
    QList<Log> batch;
    while (true)
    {
        if (mBufferLogs == nullptr)
        {
            continue;
        }
        
        int idx = mBufferLogs->indexOf('\n');
        while (idx != -1)
        {
            QByteArray raw = mBufferLogs->left(idx).trimmed();
            mBufferLogs->remove(0, idx + 1);

            if (!raw.isEmpty())
            {
                QString line = QString::fromUtf8(raw);
                Log log = mLogHelper->convertToLog(line);

                mLogHelper->updateHiddenLog(log);
                batch.append(std::move(log));
            }

            idx = mBufferLogs->indexOf('\n');
        }

        if (!batch.isEmpty())
        {
            mLogHelper->mListObjs.append(batch);
            batch.clear();
        }
    }
}

void ProcessHelper::stop()
{
    if (mThreadDetectDevices != nullptr)
    {
        Logger::d(TAG, "Stop thread detect devices");
        mThreadDetectDevices->quit();
    }
    if (mThreadRealTimeLog != nullptr)
    {
        Logger::d(TAG, "Stop thread receive real-time log");
        mThreadRealTimeLog->quit();
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

// Detect connected/disconnected devices: used in thread mThreadDetectDevices
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
        mtxCurrentDeviceIds.lock();
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
        mTimerUpdateTable->stop();
        return;
    }

    Logger::d(TAG, "Starting real-time log for device ID: " + deviceId);
    QStringList command = {"-s", deviceId, "logcat"};
    mProcessRealtimeLog->start("adb", command);
    mTimerUpdateTable->start();
}
