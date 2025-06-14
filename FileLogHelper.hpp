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
    vector<Log> mListLogs;
    QString mFilePath;
    QProcess *mProcess = nullptr;
    void readLog(const QString &filePath);
    Log convertLog(const string &line);
    void watchLog(const QString &filePath);
    void stopWatch();
    void startWatch();
    bool checkDevice();
public:
    void readLogsFromFile(const QString &filePath);
    vector<Log>& getListLogs();
    string getNameFile();
    int getSizeFile();
    bool reverseIsMarkLog(int line);
    Log getLog(int index);
    bool startWatchLog(const string &filePath);
    void clearLogs();
    QString runShellCommand(const QStringList &command_args, const QString &filePath = "");

};
#endif // FILELOGHELPER_H
