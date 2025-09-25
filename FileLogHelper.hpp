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
    QList<Log> mListLogs;
    QProcess *mProcessLogcat = nullptr;
    void readLog(const QString &filePath);
    void watchLog(const QString &filePath);
public:
    static QString mFilePath;
    bool checkPath(const QString &filePath);
    void setFilePath(const QString &filePath);
    Log convertLog(const string &line);
    QList<Log> readLogsFromFile();
    QList<Log>& getListLogs();
    QString getNameFile();
    int getSizeFile();
    bool reverseIsMarkLog(int line);
    Log getLog(int index);
};
#endif // FILELOGHELPER_H
