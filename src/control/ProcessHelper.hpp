#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H
#include <QList>
#include <QProcess>
#include <QThread>
#include <QMutex>
#include "Log.hpp"
#include "LogHelper.hpp"
#include <QTimer>
#include <memory>
class ProcessHelper
{
private:
    static const inline QString TAG = "ProcessHelper";
    static inline ProcessHelper *instance = nullptr;
    ProcessHelper();
    void detectDevices();
    void receiveRealTimeLog();

    static inline QProcess *processReadLogCat = new QProcess(nullptr);
    static inline QProcess *mProcessRealtimeLog = new QProcess(nullptr);
    QTimer *mTimerUpdateTable = new QTimer();
    QThread *mThreadDetectDevices = QThread::create([=]()
                                                    { detectDevices(); });
    QThread *mThreadRealTimeLog = QThread::create([=]()
                                                  { receiveRealTimeLog(); });

    std::shared_ptr<QByteArray> mBufferLogs = std::make_shared<QByteArray>();

    QMutex mtxCurrentDeviceIds;
    QMutex mtxLogBuffer;

    LogHelper *mLogHelper = LogHelper::getInstance();

    QStringList mCurrentDeviceIds;

public:
    static inline int mLastLogId = 0;
    static ProcessHelper *getInstance()
    {
        if (!instance)
        {
            instance = new ProcessHelper();
        }
        return instance;
    }
    QString runShellCommand(const QString program, const QStringList command_args);
    QStringList getDeviceIds();
    void clearLogcat(const QString deviceId);
    void startWatchLog(const QString filePath, const QString deviceId);
    void startWatchLogRealTime(const QString deviceId);
    class DeviceChangeListener
    {
    public:
        virtual void onDevicesIsConnected(QStringList deviceIds) = 0;
        virtual void onDevicesIsDisconnected(QStringList deviceIds) = 0;
    };
    void stop();
    void registerDeviceChangeListener(DeviceChangeListener *listener);
    void unregisterDeviceChangeListener(DeviceChangeListener *listener);

private:
    QList<DeviceChangeListener *> mDeviceChangeListeners;
};

#endif // PROCESSHANDLER_H
