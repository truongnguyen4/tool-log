#include "FilterHelper.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <optional>
#include "NotificationDialog.hpp"
#include <QProcess>

using std::string;
using std::vector;
using std::chrono::steady_clock;

const QString FilterHelper::TAG = "FilterHelper";

QList<Log> &FilterHelper::filterLogs(QList<Log> &logs, int from, int to, string pid, string tag, string msg, string level)
{
    Logger::setTimeFrom("Filter", steady_clock::now());

    if (logs.empty())
    {
        NotificationDialog::show(NotificationDialog::WARNING, "No logs to filter, returning empty list.", TAG);
        return logs;
    }

    // Convert filters to lowercase only if not empty
    if (!tag.empty())
        std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
    if (!msg.empty())
        std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower);
    if (!level.empty())
        std::transform(level.begin(), level.end(), level.begin(), ::tolower);

    Logger::d(TAG, QString::fromStdString("before filter logs size = " + std::to_string(logs.size())));

    std::optional<std::regex> pidPattern, tagPattern, msgPattern, levelPattern;
    pidPattern.reset();
    tagPattern.reset();
    msgPattern.reset();
    levelPattern.reset();
    if (!pid.empty()) { pidPattern.emplace(pid); }
    if (!tag.empty()) { tagPattern.emplace(tag); }
    if (!msg.empty()) { msgPattern.emplace(msg); }
    if (!level.empty()) { levelPattern.emplace(level); }

    for (Log &log : logs)
    {
        log.setHidden(false);

        // Line number check
        if (log.getLine() < from || log.getLine() > to) {
            log.setHidden(true);
            continue;
        }

        // PID check
        if (pidPattern && !std::regex_search(log.getPid(), *pidPattern)) {
            log.setHidden(true);
            continue;
        }

        // Tag check
        if (tagPattern) {
            string logTag = log.getTag();
            std::transform(logTag.begin(), logTag.end(), logTag.begin(), ::tolower);
            if (!std::regex_search(logTag, *tagPattern)) {
                log.setHidden(true);
                continue;
            }
        }

        // Msg check
        if (msgPattern) {
            string logMsg = log.getMsg();
            std::transform(logMsg.begin(), logMsg.end(), logMsg.begin(), ::tolower);
            if (!std::regex_search(logMsg, *msgPattern)) {
                log.setHidden(true);
                continue;
            }
        }

        // Level check
        if (levelPattern) {
            string logLevel = log.getLevel();
            std::transform(logLevel.begin(), logLevel.end(), logLevel.begin(), ::tolower);
            if (!std::regex_search(logLevel, *levelPattern)) {
                log.setHidden(true);
                continue;
            }
        }
    }
    Logger::setTimeTo("Filter", steady_clock::now());
    return logs;
}
