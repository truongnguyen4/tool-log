#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H
#include <QList>
#include <QProcess>
#include <QThread>
#include <QMutex>

class ProcessHelper {
public:
    ProcessHelper();
    static QString runShellCommand(const QString program, const QStringList command_args, int& exitCode);
    static QStringList getDeviceIds();
    static int clearLogcat(const QString deviceId);
    static int startWatchLog(const QString filePath, const QString deviceId);
    class DeviceChangeListener {
    public:
        virtual void onDevicesIsConnected(QStringList deviceIds) = 0;
        virtual void onDevicesIsDisconnected(QStringList deviceIds) = 0;
    };
    void stop();
    void registerDeviceChangeListener(DeviceChangeListener* listener);
    void unregisterDeviceChangeListener(DeviceChangeListener* listener);

private:
    static const QString TAG;
    static QProcess* processReadLogCat;
    QMutex mtxCurrentDeviceIds;
    void detectDevices();
    QThread* mThreadDetectDevices;
    QStringList mCurrentDeviceIds;
    QList<DeviceChangeListener*> mDeviceChangeListeners;
};

#endif // PROCESSHANDLER_H
