#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H
#include <QList>
#include <QProcess>
#include <QThread>
#include <QMutex>

class ProcessHelper {
public:
    static QString mDeviceId;
    static QString runShellCommand(const QString &program, const QStringList &command_args);
    static QStringList getDeviceIds();
    static int clearLogcat();
    static int startWatchLog(const QString filePath);
    class DeviceChangeListener {
    public:
        virtual void onDevicesIsConnected(QStringList deviceIds) = 0;
        virtual void onDevicesIsDisconnected(QStringList deviceIds) = 0;
    };
    void init();
    void stop();
    void registerDeviceChangeListener(DeviceChangeListener* listener);
    void unregisterDeviceChangeListener(DeviceChangeListener* listener);
private:
    static const QString TAG;
    static QProcess* process;
    QMutex mtxCurrentDeviceIds;
    void detectDevices();
    QThread* mThreadDetectDevices;
    QStringList mCurrentDeviceIds;
    QList<DeviceChangeListener*> mDeviceChangeListeners;
};

#endif // PROCESSHANDLER_H
