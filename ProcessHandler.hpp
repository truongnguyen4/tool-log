#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H
#include <QList>
#include <QProcess>
class ProcessHandler {
public:
    static QString mDeviceId;
    static QString runShellCommand(const QString &program, const QStringList &command_args);
    static QStringList getDeviceIds();
    static bool checkDeviceId(const QString deviceId);
    static int clearLogcat();
    static bool startWatchLog(const QString filePath);
private:
    static const QString TAG;
    static QProcess* process;
};

#endif // PROCESSHANDLER_H
