#ifndef LOGHELPER_HPP
#define LOGHELPER_HPP

#include <QString>
#include "Log.hpp"
#include <QRegularExpression>
#include <QList>
#include "UtilHelper.hpp"

class LogHelper
{
private:
    const QRegularExpression logcatPattern = QRegularExpression(
        R"(^\s*(\d{2}-\d{2})\s+(\d{2}:\d{2}:\d{2}\.\d+)\s+(\d+)\s+(\d+)\s+([A-Za-z])\s+(.+?)\s*:\s*(.*)$)",
        QRegularExpression::CaseInsensitiveOption);
    static const inline QString TAG = "LogHelper";
    static inline LogHelper *instance = nullptr;
    UtilHelper *mUtilHelper = UtilHelper::getInstance();

public:
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

    QList<Log> mListObjs;
    QStringList mListKeyTags;
    QStringList mListKeyLevels;
    QStringList mListPids;
    QStringList mListMsgs;
    bool tagAndOp = false;
    bool levelAndOp = false;
    bool pidAndOp = false;
    bool msgAndOp = false;

    void updateHiddenLog(Log &log);
    void filterLogs();
    Log convertToLog(const QString line);
    QStringList getLogAsFile();
};

#endif // LOGHELPER_HPP
