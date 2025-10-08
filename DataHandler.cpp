#include "DataHandler.hpp"
#include "mainwindow.h"
#include "NotificationHelper.hpp"
#include "LogHelper.hpp"
#include "Logger.hpp"
#include "Constant.hpp"

const QString DataHandler::TAG = "DataHandler";

QList<Log> DataHandler::onFilterKeyChanged(const QString &pid, const QString &tag, const QString &msg, const QString &level)
{
    bool pidAndOperation = pid.contains(Constant::LogSplit::AND);
    bool tagAndOperation = tag.contains(Constant::LogSplit::AND);
    bool msgAndOperation = msg.contains(Constant::LogSplit::AND);
    bool levelAndOperation = level.contains(Constant::LogSplit::AND);
    return LogHelper::filterLogs(LogHelper::mListLogs,
                                    1 /* from line*/,
                                    LogHelper::mListLogs.size() /* to line*/,
                                    LogHelper::splitKeywords(pid, pidAndOperation), pidAndOperation,
                                    LogHelper::splitKeywords(tag, tagAndOperation), tagAndOperation,
                                    LogHelper::splitKeywords(msg, msgAndOperation), msgAndOperation,
                                    LogHelper::splitKeywords(level, levelAndOperation), levelAndOperation);
}

QList<Log> DataHandler::refreshLog(const QString &filePath)
{
    if (!FileHelper::checkPath(filePath))
    {
        Logger::d(TAG,
                         "refreshLog");
        NotificationHelper::showError(MainWindow::ERROR_FILE_PATH);
        return QList<Log>();
    }
    LogHelper::mListLogs = mFileLogHelper.readLogsFromFile(filePath);
    return LogHelper::mListLogs;
}

int DataHandler::startWatchLog(QString filePath, const QString deviceId)
{
    return ProcessHelper::startWatchLog(filePath, deviceId);
}

int DataHandler::clearLogcat(const QString deviceId)
{
    LogHelper::mListLogs.clear();
    return ProcessHelper::clearLogcat(deviceId);
}

void DataHandler::addKey(const QString &find)
{

    if (!find.isEmpty())
    {
        if (!mListFind.contains(find))
        {
            mListFind.push_back(find);
            mFindId = mListFind.size() - 1;
        }
        else
        {
            // Swap exist key to the last
            mListFind.append(mListFind.takeAt(mListFind.indexOf(find)));
        }
    }
}
void DataHandler::addKey(const QString &pid, const QString &tag, const QString &msg, const QString &level)
{
    if (!pid.isEmpty())
    {
        if (!mListPid.contains(pid))
        {
            mListPid.push_back(pid);
            mPidId = mListPid.size() - 1;
        }
        else
        {
            // Swap exist key to the last
            mListPid.append(mListPid.takeAt(mListPid.indexOf(pid)));
        }
    }
    if (!tag.isEmpty())
    {
        if (!mListTag.contains(tag))
        {
            mListTag.push_back(tag);
            mTagId = mListTag.size() - 1;
        }
        else
        {
            // Swap exist key to the last
            mListTag.append(mListTag.takeAt(mListTag.indexOf(tag)));
        }
    }
    if (!msg.isEmpty())
    {
        if (!mListMsg.contains(msg))
        {
            mListMsg.push_back(msg);
            mMsgId = mListMsg.size() - 1;
        }
        else
        {
            // Swap exist key to the last
            mListMsg.append(mListMsg.takeAt(mListMsg.indexOf(msg)));
        }
    }
    if (!level.isEmpty())
    {
        if (!mListLevel.contains(level))
        {
            mListLevel.push_back(level);
            mLevelId = mListLevel.size() - 1;
        }
        else
        {
            // Swap exist key to the last
            mListLevel.append(mListLevel.takeAt(mListLevel.indexOf(level)));
        }
    }
}

QString DataHandler::previousKey(Ui::MainWindow *ui, QObject *obj)
{
    if (obj == ui->find)
    {
        if (!mListFind.empty())
        {
            mFindId = mFindId - 1 < 0 ? 0 : mFindId - 1;
            return mListFind.empty() ? QString() : mListFind.at(mFindId);
        }
    }
    if (obj == ui->pid)
    {
        if (!mListPid.empty())
        {
            mPidId = mPidId - 1 < 0 ? 0 : mPidId - 1;
            return mListPid.empty() ? QString() : mListPid.at(mPidId);
        }
    }
    if (obj == ui->tag)
    {
        if (!mListTag.empty())
        {
            mTagId = mTagId - 1 < 0 ? 0 : mTagId - 1;
            return mListTag.empty() ? QString() : mListTag.at(mTagId);
        }
    }
    if (obj == ui->msg)
    {
        if (!mListMsg.empty())
        {
            mMsgId = mMsgId - 1 < 0 ? 0 : mMsgId - 1;
            return mListMsg.empty() ? QString() : mListMsg.at(mMsgId);
        }
    }
    if (obj == ui->level)
    {
        if (!mListLevel.empty())
        {
            mLevelId = mLevelId - 1 < 0 ? 0 : mLevelId - 1;
            return mListLevel.empty() ? QString() : mListLevel.at(mLevelId);
        }
    }
    return "";
}

QString DataHandler::nextKey(Ui::MainWindow *ui, QObject *obj)
{
    if (obj == ui->find)
    {
        if (!mListFind.empty())
        {
            mFindId = mFindId + 1 >= mListFind.size() ? mListFind.size() - 1 : mFindId + 1;
            return mListFind.empty() ? QString() : mListFind.at(mFindId);
        }
    }
    if (obj == ui->pid)
    {
        if (!mListPid.empty())
        {
            mPidId = mPidId + 1 >= mListPid.size() ? mListPid.size() - 1 : mPidId + 1;
            return mListPid.empty() ? QString() : mListPid.at(mPidId);
        }
    }
    if (obj == ui->tag)
    {
        if (!mListTag.empty())
        {
            mTagId = mTagId + 1 >= mListTag.size() ? mListTag.size() - 1 : mTagId + 1;
            return mListTag.empty() ? QString() : mListTag.at(mTagId);
        }
    }
    if (obj == ui->msg)
    {
        if (!mListMsg.empty())
        {
            mMsgId = mMsgId + 1 >= mListMsg.size() ? mListMsg.size() - 1 : mMsgId + 1;
            return mListMsg.empty() ? QString() : mListMsg.at(mMsgId);
        }
    }
    if (obj == ui->level)
    {
        if (!mListLevel.empty())
        {
            mLevelId = mLevelId + 1 >= mListLevel.size() ? mListLevel.size() - 1 : mLevelId + 1;
            return mListLevel.empty() ? QString() : mListLevel.at(mLevelId);
        }
    }
    return "";
}
