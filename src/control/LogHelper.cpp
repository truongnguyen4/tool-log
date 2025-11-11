#include "LogHelper.hpp"
#include "Log.hpp"
#include "Logger.hpp"
#include <QProcess>
#include <QDateTime>
#include "UtilHelper.hpp"

Log LogHelper::convertToLog(const QString line)
{
    QRegularExpressionMatch match = logcatPattern.match(line);
    if (match.hasMatch())
    {
        return Log(
            match.captured(1), // date
            match.captured(2), // time
            match.captured(3), // pid
            match.captured(4), // tid
            match.captured(5), // level
            match.captured(6), // tag
            match.captured(7)  // msg
        );
    }
    Logger::w(TAG, "No match!");
    return Log(line);
}

void LogHelper::filterLogs()
{
    Logger::d(TAG, "filtering... listLogs.size = " + QString::number(mListObjs.size()));
    Logger::setTimeFrom("Filter", QDateTime::currentMSecsSinceEpoch());
    for (Log &log : mListObjs)
    {
        log.setHidden(false);
        updateHiddenLog(log);
    }
}

void LogHelper::updateHiddenLog(Log &log)
{
    if (!log.getHidden())
    {
        mUtilHelper->updateHidden<Log>(log, log.getTag(), mListKeyTags, tagAndOp);
    }
    if (!log.getHidden())
    {
        mUtilHelper->updateHidden<Log>(log, log.getMsg(), mListKeyMsgs, msgAndOp);
    }
    if (!log.getHidden())
    {
        mUtilHelper->updateHidden<Log>(log, log.getPid(), mListKeyPids, pidAndOp);
    }
    if (!log.getHidden())
    {
        mUtilHelper->updateHidden<Log>(log, log.getLevel(), mListKeyLevels, levelAndOp);
    }
    if (!log.getHidden())
    {
        mUtilHelper->updateHiddenByLine<Log>(log, QString::number(log.getLine()), mListKeyLines);
    }
    if (!log.getHidden())
    {
        mUtilHelper->updateHiddenByTime<Log>(log, log.getTime(), mListKeyTimes);
    }
}

QStringList LogHelper::getLogAsFile()
{
    QStringList logs;
    for (const Log& log : mListObjs)
    {
        logs.append(log.toString());
    }
    return logs;
}
