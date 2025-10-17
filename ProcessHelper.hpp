#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H
#include <QList>
#include <QProcess>
#include <QThread>
#include <QMutex>
#include "Log.hpp"

class ProcessHelper
{
private:
    static const QString TAG;
    static inline ProcessHelper *instance = nullptr;
    ProcessHelper();

    static inline QProcess *processReadLogCat = new QProcess(nullptr);
    static inline QProcess *mProcessRealtimeLog = new QProcess(nullptr);
    QMutex mtxCurrentDeviceIds;
    void detectDevices();
    QThread *mThreadDetectDevices;
    QStringList mCurrentDeviceIds;
    void insertLogToTable(Log log);

public:
    static QString runShellCommand(const QString program, const QStringList command_args, int &exitCode);
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
    static ProcessHelper *getInstance()
    {
        if (!instance)
        {
            instance = new ProcessHelper();
        }
        return instance;
    }

private:
    QList<DeviceChangeListener *> mDeviceChangeListeners;
};

#endif // PROCESSHANDLER_H
