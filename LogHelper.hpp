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
    static inline LogHelper *instance = nullptr;

public:
    static LogHelper *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new LogHelper();
        }
        return instance;
    }

    QList<Log> mListLogs;
    QStringList mTags;
    QStringList mLevels;
    QStringList mPids;
    QStringList mMsgs;
    bool tagAndOperation = false;
    bool levelAndOperation = false;
    bool pidAndOperation = false;
    bool msgAndOperation = false;

    void filterLogs(QList<Log> &logs);

    void updateHiddenLog(Log &log);
    static void updateHiddenLogByTag(Log &log, const QStringList tags, const bool andOperation);
    static void updateHiddenLogByMsg(Log &log, const QStringList messages, const bool andOperation);
    static void updateHiddenLogByLevel(Log &log, const QStringList levels, const bool andOperation);
    static void updateHiddenLogByPid(Log &log, const QStringList pids, const bool andOperation);
    static Log convertToLog(const QString line);
};

#endif // LOGHELPER_HPP
