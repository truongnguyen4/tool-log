#ifndef LOGHELPER_HPP
#define LOGHELPER_HPP

#include <QString>
#include "Log.hpp"
#include <QRegularExpression>
#include <QList>
#include <functional>
#include "UtilHelper.hpp"

class LogHelper
{
private:
    static const inline QString TAG = "LogHelper";
    static inline LogHelper *instance = nullptr;
    const QRegularExpression logcatPattern_1 = QRegularExpression(
        R"(^\s*(\d{2}-\d{2})\s+(\d{2}:\d{2}:\d{2}\.\d+)\s+(\d+)\s+(\d+)\s+([A-Za-z])\s+(.+?)\s*:\s*(.*)$)",
        QRegularExpression::CaseInsensitiveOption);
    const QRegularExpression logcatPattern_2 = QRegularExpression(
        R"((\d{2}-\d{2})\s+(\d{2}:\d{2}:\d{2}\.\d{3})\s+(\S+)\s+([VDIWEFS])\/([^:]+):\s*(.*))",
        QRegularExpression::CaseInsensitiveOption);
    UtilHelper *mUtilHelper = UtilHelper::getInstance();

    Log convertToLogFormat1(const QString &line);
    Log convertToLogFormat2(const QString &line);
    std::function<Log(const QString &)> convertToLogFunc;

public:
    LogHelper() {
        convertToLogFunc = [this](const QString &line) { return convertToLogFormat1(line); };
    }
    static LogHelper *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new LogHelper();
        }
        return instance;
    }
    void clearLogs()
    {
        Log::static_id = 0;
        mListObjs.clear();
    }

    static bool useFormat1;
    QList<Log> mListObjs;
    QStringList mListKeyTags;
    QStringList mListKeyLevels;
    QStringList mListKeyPids;
    QStringList mListKeyMsgs;
    QStringList mListKeyLines;
    QStringList mListKeyTimes;
    bool tagAndOp = false;
    bool levelAndOp = false;
    bool pidAndOp = false;
    bool msgAndOp = false;

    void updateHiddenLog(Log &log);
    void filterLogs();
    Log convertToLog(const QString &line);
    QStringList getLogAsFile();
    void toggleFormatLog();
};

#endif // LOGHELPER_HPP
