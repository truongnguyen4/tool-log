#ifndef FILELOGHELPER_H
#define FILELOGHELPER_H
#include <vector>
#include "Log.hpp"
#include <regex>
#include <QProcess>

using std::string;
using std::vector;
using std::regex;
class FileLogHelper : public QObject
{
    Q_OBJECT
private:
    static const QString TAG;
    static const regex logcatPattern;
    static std::atomic_bool isWatchingLog;
    QList<Log> mListLogs;
    QString mFilePath;
    QProcess *mProcessLogcat = nullptr;
    void readLog(const QString &filePath);
    void watchLog(const QString &filePath);
    QString checkDevice();
    bool checkPath(const string &filePath);
public:
    bool setFilePath(const QString &filePath);
    Log convertLog(const string &line);
    QList<Log> readLogsFromFile(const QString &filePath);
    QList<Log>& getListLogs();
    string getNameFile();
    int getSizeFile();
    bool reverseIsMarkLog(int line);
    Log getLog(int index);
    bool startWatchLog(QProcess *& process);
    void clearLogs();
    QString runShellCommand(const QStringList &command_args);

};
#endif // FILELOGHELPER_H
