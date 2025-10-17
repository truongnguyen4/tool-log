#include "DataHandler.hpp"
#include "mainwindow.h"
#include "NotificationHelper.hpp"
#include "LogHelper.hpp"
#include "Logger.hpp"
#include "Constant.hpp"
#include "PropertyHelper.hpp"
#include "SettingHelper.hpp"
#include "UtilHelper.hpp"

const QString DataHandler::TAG = "DataHandler";

void DataHandler::filterLogs(const QString &pid, const QString &tag, const QString &msg, const QString &level)
{
    LogHelper *logHelper = LogHelper::getInstance();
    logHelper->mTags = UtilHelper::splitKeywords(tag);
    logHelper->mLevels = UtilHelper::splitKeywords(level);
    logHelper->mPids = UtilHelper::splitKeywords(pid);
    logHelper->mMsgs = UtilHelper::splitKeywords(msg);
    logHelper->tagAndOperation = tag.contains(Constant::Split::AND);
    logHelper->levelAndOperation = level.contains(Constant::Split::AND);
    logHelper->pidAndOperation = pid.contains(Constant::Split::AND);
    logHelper->msgAndOperation = msg.contains(Constant::Split::AND);

    logHelper->filterLogs(logHelper->mListLogs);
}

void DataHandler::refreshLog(const QString &filePath)
{
    if (!FileHelper::checkPath(filePath))
    {
        NotificationHelper::showError(MainWindow::ERROR_FILE_PATH);
    }
    LogHelper::getInstance()->mListLogs = FileHelper::readLogsFromFile(filePath);
}

void DataHandler::startWatchLog(QString filePath, const QString deviceId)
{
    ProcessHelper::getInstance()->startWatchLog(filePath, deviceId);
}

void DataHandler::startWatchLogRealTime(const QString deviceId)
{
    ProcessHelper::getInstance()->startWatchLogRealTime(deviceId);
}

void DataHandler::clearLogcat(const QString deviceId)
{
    LogHelper::getInstance()->mListLogs.clear();
    ProcessHelper::getInstance()->clearLogcat(deviceId);
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

void DataHandler::addKey(const QString &property, const QString &setting)
{
    if (!property.isEmpty())
    {
        if (!mListProperty.contains(property))
        {
            Logger::d(TAG, "Add property key: " + property);
            mListProperty.push_back(property);
        }
    }
    if (!setting.isEmpty())
    {
        if (!mListSetting.contains(setting))
        {
            Logger::d(TAG, "Add setting key: " + setting);
            mListSetting.push_back(setting);
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
    if (obj == ui->property_filter)
    {
        if (!mListProperty.empty())
        {
            Logger::d(TAG, "Previous property key: " + mListProperty.at(mPropertyId));
            mPropertyId = mPropertyId - 1 < 0 ? 0 : mPropertyId - 1;
            return mListProperty.empty() ? QString() : mListProperty.at(mPropertyId);
        }
    }
    if (obj == ui->setting_filter)
    {
        if (!mListSetting.empty())
        {
            mSettingId = mSettingId - 1 < 0 ? 0 : mSettingId - 1;
            Logger::d(TAG, "Previous setting key: " + mListSetting.at(mSettingId));
            return mListSetting.empty() ? QString() : mListSetting.at(mSettingId);
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
    if (obj == ui->property_filter)
    {
        if (!mListProperty.empty())
        {
            mPropertyId = mPropertyId + 1 >= mListProperty.size() ? mListProperty.size() - 1 : mPropertyId + 1;
            return mListProperty.empty() ? QString() : mListProperty.at(mPropertyId);
        }
    }
    if (obj == ui->setting_filter)
    {
        if (!mListSetting.empty())
        {
            mSettingId = mSettingId + 1 >= mListSetting.size() ? mListSetting.size() - 1 : mSettingId + 1;
            return mListSetting.empty() ? QString() : mListSetting.at(mSettingId);
        }
    }
    return "";
}

void DataHandler::loadProperties(const QString deviceId)
{
    PropertyHelper::getInstance()->loadProperties(deviceId);
}

void DataHandler::loadSettings(const QString deviceId)
{
    SettingHelper::getInstance()->loadSettings(deviceId);
}

void DataHandler::filterSettings(const QString name)
{
    const QStringList names = UtilHelper::splitKeywords(name);
    SettingHelper::getInstance()->filterSettings(SettingHelper::getInstance()->mListSettings, names);
}

void DataHandler::filterProperties(const QString name)
{
    QStringList names = UtilHelper::splitKeywords(name);
    PropertyHelper::getInstance()->filterProperty(PropertyHelper::getInstance()->mListProperties, names);
}
