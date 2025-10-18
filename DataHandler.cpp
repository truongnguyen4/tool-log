#include "DataHandler.hpp"
#include "mainwindow.h"
#include "Logger.hpp"
#include "UtilHelper.hpp"

void DataHandler::filterLogs(const QString &pid, const QString &tag, const QString &msg, const QString &level)
{
    Logger::d(TAG, "filterLogs called with pid: " + pid + ", tag: " + tag + ", msg: " + msg + ", level: " + level);
    mUtilHelper->addKey(mListPid, mPidId, pid);
    mUtilHelper->addKey(mListTag, mTagId, tag);
    mUtilHelper->addKey(mListMsg, mMsgId, msg);
    mUtilHelper->addKey(mListLevel, mLevelId, level);

    mUtilHelper->updateFilter(mLogHelper->mListPids, mLogHelper->pidAndOp, pid);
    mUtilHelper->updateFilter(mLogHelper->mListKeyTags, mLogHelper->tagAndOp, tag);
    mUtilHelper->updateFilter(mLogHelper->mListMsgs, mLogHelper->msgAndOp, msg);
    mUtilHelper->updateFilter(mLogHelper->mListKeyLevels, mLogHelper->levelAndOp, level);

    mLogHelper->filterLogs();
}

void DataHandler::filterSettings(const QString setting)
{
    mUtilHelper->addKey(mListSetting, mSettingId, setting);
    mUtilHelper->updateFilter(mSettingHelper->mListKeys, mSettingHelper->andOp, setting);
    mSettingHelper->filterSettings();
}

void DataHandler::filterProperties(const QString property)
{
    mUtilHelper->addKey(mListProperty, mPropertyId, property);
    mUtilHelper->updateFilter(mPropertyHelper->mListKeys, mPropertyHelper->andOp, property);
    mPropertyHelper->filterProperties();
}

void DataHandler::loadLogs(const QString &filePath)
{
    if (!mFileHelper->checkPath(filePath))
    {
        return;
    }
    mUtilHelper->clearListData<Log>(Log::static_id, mLogHelper->mListObjs);
    mFileHelper->readLogsFromFile(filePath);
}

void DataHandler::loadSettings(const QString deviceId)
{
    mUtilHelper->clearListData<Setting>(Setting::static_id, mSettingHelper->mListObjs);
    mSettingHelper->loadSettings(deviceId);
}

void DataHandler::loadProperties(const QString deviceId)
{
    mUtilHelper->clearListData<Property>(Property::static_id, mPropertyHelper->mListObjs);
    mPropertyHelper->loadProperties(deviceId);
}

void DataHandler::clearLogcat(const QString deviceId)
{
    mUtilHelper->clearListData<Log>(Log::static_id, mLogHelper->mListObjs);
    ProcessHelper::mLastLogId = 0;
    mProcessHelper->clearLogcat(deviceId);
}

void DataHandler::startWatchLog(QString filePath, const QString deviceId)
{
    mProcessHelper->startWatchLog(filePath, deviceId);
}

void DataHandler::startWatchLogRealTime(const QString deviceId, const bool isWatching)
{
    if (isWatching)
    {
        mUtilHelper->clearListData<Log>(Log::static_id, mLogHelper->mListObjs);
    }
    mProcessHelper->startWatchLogRealTime(deviceId);
}

QString DataHandler::previousKey(Ui::MainWindow *ui, QObject *obj)
{
    auto findPrevious = [](const QStringList &keys, int &index) -> QString
    {
        if (!keys.empty())
        {
            index = index - 1 < 0 ? 0 : index - 1;
            return keys.at(index);
        }
        return QString();
    };

    if (obj == ui->find)
    {
        return findPrevious(mListFind, mFindId);
    }
    if (obj == ui->pid)
    {
        return findPrevious(mListPid, mPidId);
    }
    if (obj == ui->tag)
    {
        return findPrevious(mListTag, mTagId);
    }
    if (obj == ui->msg)
    {
        return findPrevious(mListMsg, mMsgId);
    }
    if (obj == ui->level)
    {
        return findPrevious(mListLevel, mLevelId);
    }
    if (obj == ui->property_filter)
    {
        return findPrevious(mListProperty, mPropertyId);
    }
    if (obj == ui->setting_filter)
    {
        return findPrevious(mListSetting, mSettingId);
    }

    return QString();
}

QString DataHandler::nextKey(Ui::MainWindow *ui, QObject *obj)
{
    auto findNext = [](const QStringList &keys, int &index) -> QString
    {
        if (!keys.empty())
        {
            index = index + 1 >= keys.size() ? keys.size() - 1 : index + 1;
            return keys.at(index);
        }
        return QString();
    };

    if (obj == ui->find)
    {
        return findNext(mListFind, mFindId);
    }
    if (obj == ui->pid)
    {
        return findNext(mListPid, mPidId);
    }
    if (obj == ui->tag)
    {
        return findNext(mListTag, mTagId);
    }
    if (obj == ui->msg)
    {
        return findNext(mListMsg, mMsgId);
    }
    if (obj == ui->level)
    {
        return findNext(mListLevel, mLevelId);
    }
    if (obj == ui->property_filter)
    {
        return findNext(mListProperty, mPropertyId);
    }
    if (obj == ui->setting_filter)
    {
        return findNext(mListSetting, mSettingId);
    }
    return QString();
}

void DataHandler::saveLog(const QString filePath)
{
    Logger::d(TAG, "Save log to file: " + filePath);
    QStringList logs = mLogHelper->getLogAsFile();
    mFileHelper->saveToFile(filePath, logs);
}
