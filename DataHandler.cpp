#include "DataHandler.hpp"

vector<Log> &DataHandler::onFilterKeyChanged(const QString &pid, const QString &tag, const QString &msg, const QString &level)
{
    addKey(pid, tag, msg, level);
    return mFilterLogHelper.filterLogs(mFileLogHelper.getListLogs(), 1, mFileLogHelper.getSizeFile(), pid.toStdString(), tag.toStdString(), msg.toStdString(), level.toStdString());
}

FileLogHelper &DataHandler::getFileLogHelper()
{
    return mFileLogHelper;
}

vector<Log> DataHandler::refreshLog(const QString &file)
{
    mFileLogHelper.readLogsFromFile(file);
    return mFileLogHelper.getListLogs();
}

bool DataHandler::startWatchLog(QString file)
{
    return mFileLogHelper.startWatchLog(file.toStdString());
}

void DataHandler::clearLogs()
{
    mFileLogHelper.clearLogs();
}

void DataHandler::addKey(const QString &pid, const QString &tag, const QString &msg, const QString &level)
{
    if (pid != "")
    {
        if ((mListPid.size() > 0 && pid != mListPid.at(mPidId))
            || mPidId == mListPid.size() - 1
            || mListPid.size() == 0)
        {
            mListPid.push_back(pid);
            mPidId = mListPid.size() - 1;
        }
    }
    if (tag != "")
    {
        if ((mListTag.size() > 0 && tag != mListTag.at(mTagId))
            || mTagId == mListTag.size() - 1
            || mListTag.size() == 0)
        {
            mListTag.push_back(tag);
            mTagId = mListTag.size() - 1;
        }
    }
    if (msg != "")
    {
        if ((mListMsg.size() > 0 && msg != mListMsg.at(mMsgId))
            || mMsgId == mListMsg.size() - 1
            || mListMsg.size() == 0)
        {
            mListMsg.push_back(msg);
            mMsgId = mListMsg.size() - 1;
        }
    }
    if (level != "")
    {
        if ((mListLevel.size() > 0 && level != mListLevel.at(mLevelId))
            || mLevelId == mListLevel.size() - 1
            || mListLevel.size() == 0)
        {
            mListLevel.push_back(level);
            mLevelId = mListLevel.size() - 1;
        }
    }
}

QString DataHandler::previousKey(Ui::MainWindow *ui, QObject *obj)
{
    if (obj == ui->pid)
    {
        mPidId = mPidId - 1 < 0 ? 0 : mPidId - 1;
        Logger::d("DataHandler", QString::fromStdString("previousKey -> pid = " + std::to_string(mPidId)));
        return mListPid.empty() ? "" : mListPid.at(mPidId);
    }
    if (obj == ui->tag)
    {
        mTagId = mTagId - 1 < 0 ? 0 : mTagId - 1;
        return mListTag.empty() ? "" : mListTag.at(mTagId);
    }
    if (obj == ui->msg)
    {
        mMsgId = mMsgId - 1 < 0 ? 0 : mMsgId - 1;
        return mListMsg.empty() ? "" : mListMsg.at(mMsgId);
    }
    if (obj == ui->level)
    {
        mLevelId = mLevelId - 1 < 0 ? 0 : mLevelId - 1;
        return mListLevel.empty() ? "" : mListLevel.at(mLevelId);
    }
    return "";
}

QString DataHandler::nextKey(Ui::MainWindow *ui, QObject *obj)
{
    if (obj == ui->pid)
    {
        mPidId = mPidId + 1 >= mListPid.size() ? mListPid.size() - 1 : mPidId + 1;
        Logger::d("DataHandler", QString::fromStdString("nextKey -> pid = " + std::to_string(mPidId)));
        return mListPid.at(mPidId);
    }
    if (obj == ui->tag)
    {
        mTagId = mTagId + 1 >= mListTag.size() ? mListTag.size() - 1 : mTagId + 1;
        return mListTag.at(mTagId);
    }
    if (obj == ui->msg)
    {
        mMsgId = mMsgId + 1 >= mListMsg.size() ? mListMsg.size() - 1 : mMsgId + 1;
        return mListMsg.at(mMsgId);
    }
    if (obj == ui->level)
    {
        mLevelId = mLevelId + 1 >= mListLevel.size() ? mListLevel.size() - 1 : mLevelId + 1;
        return mListLevel.at(mLevelId);
    }
    return "";
}
