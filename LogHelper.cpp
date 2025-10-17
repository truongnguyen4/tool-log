#include "LogHelper.hpp"
#include "Log.hpp"
#include "Logger.hpp"
#include <QProcess>
#include <QDateTime>
#include "Constant.hpp"

const QString LogHelper::TAG = "LogHelper";
const QRegularExpression LogHelper::logcatPattern(
    R"(^\s*(\d{2}-\d{2})\s+(\d{2}:\d{2}:\d{2}\.\d+)\s+(\d+)\s+(\d+)\s+([A-Za-z])\s+(.+?)\s*:\s*(.*)$)",
    QRegularExpression::CaseInsensitiveOption);

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

void LogHelper::filterLogs(QList<Log> &logs)
{
    Logger::setTimeFrom("Filter", QDateTime::currentMSecsSinceEpoch());
    for (Log &log : logs)
    {
        log.setHidden(false);
        LogHelper::updateHiddenLog(log);
    }
    Logger::setTimeTo("Filter", QDateTime::currentMSecsSinceEpoch());
}

void LogHelper::updateHiddenLog(Log &log)
{
    if (!log.getHidden())
    {
        LogHelper::updateHiddenLogByTag(log, mTags, tagAndOperation);
    }
    if (!log.getHidden())
    {
        LogHelper::updateHiddenLogByLevel(log, mLevels, levelAndOperation);
    }
    if (!log.getHidden())
    {
        LogHelper::updateHiddenLogByPid(log, mPids, pidAndOperation);
    }
    if (!log.getHidden())
    {
        LogHelper::updateHiddenLogByMsg(log, mMsgs, msgAndOperation);
    }
}

void LogHelper::updateHiddenLogByTag(Log &log, const QStringList tags, const bool andOperation)
{
    if (tags.isEmpty())
    {
        return;
    }
    QString logTag = log.getTag();
    if (andOperation)
    {
        bool isHidden = false;
        for (const QString &tag : tags)
        {
            if (!logTag.contains(tag, Qt::CaseInsensitive))
            {
                isHidden = true;
                break;
            }
        }
        log.setHidden(isHidden);
    }
    else
    {
        bool isHidden = true;
        for (const QString &tag : tags)
        {
            if (logTag.contains(tag, Qt::CaseInsensitive))
            {
                isHidden = false;
                break;
            }
        }
        log.setHidden(isHidden);
    }
}

void LogHelper::updateHiddenLogByMsg(Log &log, const QStringList messages, const bool andOperation)
{
    if (messages.isEmpty())
    {
        return;
    }
    QString logMsg = log.getMsg();
    if (andOperation)
    {
        bool isHidden = false;
        for (const QString &message : messages)
        {
            if (!logMsg.contains(message, Qt::CaseInsensitive))
            {
                isHidden = true;
                break;
            }
        }
        log.setHidden(isHidden);
    }
    else
    {
        bool isHidden = true;
        for (const QString &message : messages)
        {
            if (logMsg.contains(message, Qt::CaseInsensitive))
            {
                isHidden = false;
                break;
            }
        }
        log.setHidden(isHidden);
    }
}

void LogHelper::updateHiddenLogByLevel(Log &log, const QStringList levels, const bool andOperation)
{
    if (levels.isEmpty())
    {
        return;
    }
    QString logLevel = log.getLevel();
    if (andOperation)
    {
        bool isHidden = false;
        for (const QString &level : levels)
        {
            if (!logLevel.contains(level, Qt::CaseInsensitive))
            {
                isHidden = true;
                break;
            }
        }
        log.setHidden(isHidden);
    }
    else
    {
        bool isHidden = true;
        for (const QString &level : levels)
        {
            if (logLevel.contains(level, Qt::CaseInsensitive))
            {
                isHidden = false;
                break;
            }
        }
        log.setHidden(isHidden);
    }
}

void LogHelper::updateHiddenLogByPid(Log &log, const QStringList pids, const bool andOperation)
{
    if (pids.isEmpty())
    {
        return;
    }
    QString logPid = log.getPid();
    if (andOperation)
    {
        bool isHidden = false;
        for (const QString &pid : pids)
        {
            if (!logPid.contains(pid, Qt::CaseInsensitive))
            {
                isHidden = true;
                break;
            }
        }
        log.setHidden(isHidden);
    }
    else
    {
        bool isHidden = true;
        for (const QString &pid : pids)
        {
            if (logPid.contains(pid, Qt::CaseInsensitive))
            {
                isHidden = false;
                break;
            }
        }
        log.setHidden(isHidden);
    }
}
