#include "LogHelper.hpp"
#include "Log.hpp"
#include "Logger.hpp"
#include <QProcess>
#include <QDateTime>

const QString LogHelper::TAG = "LogHelper";
const QRegularExpression LogHelper::logcatPattern(
    R"(^\s*(\d{2}-\d{2})\s+(\d{2}:\d{2}:\d{2}\.\d+)\s+(\d+)\s+(\d+)\s+([A-Za-z])\s+(.+?)\s*:\s*(.*)$)",
    QRegularExpression::CaseInsensitiveOption
);
QList<Log> LogHelper::mListLogs = QList<Log>();

Log LogHelper::convertToLog(const QString line)
{
    QRegularExpressionMatch match = logcatPattern.match(line);
    Log log;
    if (match.hasMatch()) {
        log.setDate(match.captured(1));
        log.setTime(match.captured(2));
        log.setPid(match.captured(3));
        log.setTid(match.captured(4));
        log.setLevel(match.captured(5));
        log.setTag(match.captured(6));
        log.setMsg(match.captured(7));
    } else {
        Logger::w(TAG, "No match!");
        log.setMsg(match.captured(7));
    }
    return log;
}

QList<Log> LogHelper::filterLogs(QList<Log> logs, int from, int to, QStringList pids, QStringList tags, QStringList messages, QStringList levels)
{
    Logger::setTimeFrom("Filter",  QDateTime::currentMSecsSinceEpoch());

    for (Log &log : logs)
    {
        log.setHidden(false);
    }
    Logger::d(TAG, "No filters applied, all logs are visible.");


    if (!tags.empty())
    {
        Logger::d(TAG, "Filtering logs by tags: " + tags.join(", "));
        logs = filterLogsByTag(logs, tags);
    }

    if (!pids.empty())
    {
        logs = filterLogsByPid(logs, pids);
    }

    if (!messages.empty())
    {
        logs = filterLogsByMsg(logs, messages);
    }

    if (!levels.empty())
    {
        logs = filterLogsByLevel(logs, levels);
    }

    // filterLogsByLine(logs, from, to);

    Logger::setTimeTo("Filter",  QDateTime::currentMSecsSinceEpoch());
    return logs;
}

QList<Log> LogHelper::filterLogsByTag(QList<Log> logs, QStringList tags) {
    Logger::d(TAG, "Filtering logs by tags: " + tags.join(", "));
    for (Log &log : logs)
    {
        if (log.getHidden())
        {
            continue;
        }
        QString logTag = log.getTag();
        bool isHidden = true;
        for (const QString &tag : tags) {
            if (logTag.contains(tag, Qt::CaseInsensitive)) {
                isHidden = false;
                break;
            }
        }
        log.setHidden(isHidden);
    }
    return logs;
}

QList<Log> LogHelper::filterLogsByMsg(QList<Log> logs, QStringList messages) {
    Logger::d(TAG, "Filtering logs by messages: " + messages.join(", "));
    for (Log &log : logs)
    {
        if (log.getHidden())
        {
            continue;
        }
        QString logMsg = log.getMsg();
        bool isHidden = true;
        for (const QString &message : messages) {
            if (logMsg.contains(message, Qt::CaseInsensitive)) {
                isHidden = false;
                break;
            }
        }
        log.setHidden(isHidden);
    }
    return logs;
}

QList<Log> LogHelper::filterLogsByLevel(QList<Log> logs, QStringList levels) {
    Logger::d(TAG, "Filtering logs by levels: " + levels.join(", "));
    for (Log &log : logs)
    {
        if (log.getHidden())
        {
            continue;
        }
        QString logLevel = log.getLevel();
        bool isHidden = true;
        for (const QString &level : levels) {
            if (!logLevel.contains(level, Qt::CaseInsensitive)) {
                isHidden = false;
                break;
            }
        }
        log.setHidden(isHidden);
    }
    return logs;
}

QList<Log> LogHelper::filterLogsByPid(QList<Log> logs, QStringList pids) {
    Logger::d(TAG, "Filtering logs by PIDs: " + pids.join(", "));
    for (Log &log : logs)
    {
        if (log.getHidden())
        {
            continue;
        }
        QString logPid = log.getPid();
        bool isHidden = true;
        for (const QString &pid : pids) {
            if (!logPid.contains(pid, Qt::CaseInsensitive)) {
                isHidden = false;
                break;
            }
        }
        log.setHidden(true);
    }
    return logs;
}

QList<Log> LogHelper::filterLogsByLine(QList<Log> logs, int from, int to) {
    Logger::d(TAG, QString::fromStdString("Filtering logs by line from " + std::to_string(from) + " to " + std::to_string(to)));
    for (Log &log : logs)
    {
        if (log.getHidden())
        {
            continue;
        }
        if (log.getLine() < from || log.getLine() > to) {
            log.setHidden(true);
        }
    }
    return logs;
}

