#include "ProcessHelper.hpp"
#include <QProcess>
#include "Logger.hpp"
#include "mainwindow.h"
#include <QThread>

QString const ProcessHelper::TAG = "ProcessHelper";
QProcess* ProcessHelper::process = new QProcess();
QString ProcessHelper::mDeviceId = "";

void ProcessHelper::registerDeviceChangeListener(DeviceChangeListener* listener) {
    if (listener == nullptr) {
        return;
    }
    if (!mDeviceChangeListeners.contains(listener)) {
        Logger::d(TAG, "Register new device change listener");
        mDeviceChangeListeners.append(listener);

        QMutexLocker locker(&mtxCurrentDeviceIds);
        mCurrentDeviceIds = getDeviceIds();
        listener->onDevicesIsConnected(mCurrentDeviceIds); // Initial call to provide current devices
        locker.unlock();
    }
}

void ProcessHelper::unregisterDeviceChangeListener(DeviceChangeListener* listener) {
    Logger::d(TAG, "Unregister device change listener");
    mDeviceChangeListeners.removeAll(listener);
}

QString ProcessHelper::runShellCommand(const QString &program, const QStringList &args)
{
    QProcess *process = new QProcess();
    process->start(program, args);
    process->waitForFinished();

    Logger::d(TAG, "runShellCommand: " + program + " " + ", PID = " + QString::number(process->processId()) + ", DEVICE ID = " + ProcessHelper::mDeviceId);
    QString output = process->readAllStandardOutput();
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
        for (int i=1; i<lines.length(); i++)
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

int ProcessHelper::clearLogcat()
{
    if (ProcessHelper::mDeviceId.isEmpty())
    {
        Logger::d(TAG, "Can't clear log due to DEVICE ID is empty.");
        return MainWindow::ERROR_DEVICE_ID;
    }
    runShellCommand("adb", QStringList() << "-s" << ProcessHelper::mDeviceId << "logcat" << "-c");
    return MainWindow::SUCCESS;
}

int ProcessHelper::startWatchLog(const QString filePath)
{
    // Check if the file is already being watched, stop the current watch
    if (process->state() == QProcess::Running)
    {
        ProcessHelper::process->kill();
        ProcessHelper::process->waitForFinished();
        Logger::d(TAG, "Stop watching log");
        return MainWindow::SUCCESS;
    }

    if (ProcessHelper::mDeviceId.isEmpty())
    {
        Logger::d(TAG, "Can't start watching device due to DEVICE ID is empty.");
        return MainWindow::ERROR_DEVICE_ID;
    }
    ProcessHelper::process->setStandardOutputFile(filePath, QIODevice::NewOnly);
    ProcessHelper::process->start("adb", QStringList() << "-s" << ProcessHelper::mDeviceId << "logcat");
    Logger::d(TAG, "Start watching log, PID = " + QString::number(process->processId()) + ", DEVICE ID = " + ProcessHelper::mDeviceId);

    return MainWindow::SUCCESS;
}

void ProcessHelper::init()
{
    mThreadDetectDevices = QThread::create([=]() {
        detectDevices();
    });

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
    QMutexLocker locker(&mtxCurrentDeviceIds);
    mCurrentDeviceIds = getDeviceIds();
    locker.unlock();

    while (true)
    {
        QThread::sleep(1); // delay 1 seconds
        QStringList newDeviceIds = getDeviceIds();
        locker.relock();
        if (newDeviceIds == mCurrentDeviceIds)
        {
            locker.unlock();
            continue;
        }
        if (newDeviceIds.size() > mCurrentDeviceIds.size()) {
            QStringList addedDevices = newDeviceIds;
            for (const QString& device : mCurrentDeviceIds) {
                addedDevices.removeAll(device);
            }
            Logger::d(TAG, "Device connected: " + addedDevices.join(", "));

            for (DeviceChangeListener *listener : mDeviceChangeListeners) {
                listener->onDevicesIsConnected(addedDevices);
            }
        }

        if (newDeviceIds.size() < mCurrentDeviceIds.size()) {
            QStringList removedDevices = mCurrentDeviceIds;
            for (const QString& device : newDeviceIds) {
                removedDevices.removeAll(device);
            }
            Logger::d(TAG, "Device disconnected: " + removedDevices.join(", "));

            for (DeviceChangeListener *listener : mDeviceChangeListeners) {
                listener->onDevicesIsDisconnected(removedDevices);
            }
        }
        Logger::d(TAG, "Update mCurrentDeviceIds");
        mCurrentDeviceIds = newDeviceIds;
        locker.unlock();
    }
}
