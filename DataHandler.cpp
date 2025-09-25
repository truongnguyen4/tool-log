#include "DataHandler.hpp"
#include "mainwindow.h"
#include "NotificationManager.hpp"

const QString DataHandler::TAG = "DataHandler";

QList<Log> &DataHandler::onFilterKeyChanged(const QString &pid, const QString &tag, const QString &msg, const QString &level)
{
    addKey(pid, tag, msg, level);
    return mFilterLogHelper.filterLogs(mFileLogHelper.getListLogs(), 1, mFileLogHelper.getSizeFile(), pid.toStdString(), tag.toStdString(), msg.toStdString(), level.toStdString());
}

FileLogHelper &DataHandler::getFileLogHelper()
{
    return mFileLogHelper;
}

QList<Log> DataHandler::refreshLog(const QString &filePath)
{
    if (!mFileLogHelper.checkPath(filePath))
    {
        Logger::d(TAG, "refreshLog");
        NotificationManager::showError(MainWindow::ERROR_FILE_PATH);
        return QList<Log>();
    }

    return mFileLogHelper.readLogsFromFile();
}

int DataHandler::startWatchLog(QString filePath, const QString deviceId)
{
    if (!mFileLogHelper.checkPath(filePath))
    {
        return MainWindow::ERROR_FILE_PATH;
    }

    if (!ProcessHandler::checkDeviceId(deviceId))
    {
        return MainWindow::ERROR_DEVICE_ID;
    }
    
    return ProcessHandler::startWatchLog(filePath);
}

int DataHandler::clearLogcat(const QString deviceId)
{
    if (!ProcessHandler::checkDeviceId(deviceId))
    {
        return MainWindow::ERROR_DEVICE_ID;
    }

    return ProcessHandler::clearLogcat();
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
        Logger::d(TAG, "previousKey -> pid = " + QString::number(mPidId));
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
        Logger::d(TAG, "nextKey -> pid = " + QString::number(mPidId));
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

QStringList DataHandler::getDeviceIds()
{
    return ProcessHandler::getDeviceIds();
}


int DataHandler::deviceIdChanged(const QString deviceId)
{
    if (!ProcessHandler::checkDeviceId(deviceId))
    {
        return MainWindow::ERROR_DEVICE_ID;
    }
    return MainWindow::SUCCESS;
}
