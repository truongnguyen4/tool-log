#ifndef LOGHELPER_HPP
#define LOGHELPER_HPP

#include <QString>
#include "Log.hpp"
#include <QRegularExpression>

class LogHelper
{
private:
    static const QRegularExpression logcatPattern;
    static const QString TAG;
    
public:
    static QList<Log> mListLogs;
    static Log convertToLog(const QString line);
    static QList<Log> filterLogs(QList<Log> logs, int from, int to,
                                const QStringList pid, const bool pidAndOperation,
                                const QStringList tag, const bool tagAndOperation,
                                const QStringList msg, const bool msgAndOperation,
                                const QStringList level, const bool levelAndOperation);
    static QList<Log> filterLogsByTag(QList<Log> logs, const QStringList tag, const bool andOperate);
    static QList<Log> filterLogsByMsg(QList<Log> logs, const QStringList msg, const bool andOperate);
    static QList<Log> filterLogsByLevel(QList<Log> logs, const QStringList level, const bool andOperate);
    static QList<Log> filterLogsByPid(QList<Log> logs, const QStringList pid, const bool andOperate);
    static QList<Log> filterLogsByLine(QList<Log> logs, const int from, const int to);
    static QStringList splitKeywords(const QString key, const bool andOperation);
    static QStringList splitKeywordsByOr(const QString key);
    static QStringList splitKeywordsByAnd(const QString key);
};

#endif // LOGHELPER_HPP
