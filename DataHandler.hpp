#ifndef DATAHANDLER_H
#define DATAHANDLER_H
#include "FileHelper.hpp"
#include "ui_mainwindow.h"
#include <QString>
#include "ProcessHelper.hpp"
#include "Property.hpp"
#include "Setting.hpp"
#include "LogHelper.hpp"
#include "PropertyHelper.hpp"
#include "SettingHelper.hpp"
class DataHandler
{
private:
    static const inline QString TAG = "DataHandler";
    static inline DataHandler *mDataHandler = nullptr;
    DataHandler() {};

    PropertyHelper *mPropertyHelper = PropertyHelper::getInstance();
    LogHelper *mLogHelper = LogHelper::getInstance();
    SettingHelper *mSettingHelper = SettingHelper::getInstance();
    ProcessHelper *mProcessHelper = ProcessHelper::getInstance();
    FileHelper *mFileHelper = FileHelper::getInstance();
    UtilHelper *mUtilHelper = UtilHelper::getInstance();

public:
    QStringList mListFind;
    QStringList mListMsg;
    QStringList mListTag;
    QStringList mListPid;
    QStringList mListLevel;
    QStringList mListProperty;
    QStringList mListSetting;
    int mFindId = 0;
    int mMsgId = 0;
    int mTagId = 0;
    int mPidId = 0;
    int mLevelId = 0;
    int mPropertyId = 0;
    int mSettingId = 0;
    static DataHandler *getInstance()
    {
        if (!mDataHandler)
        {
            mDataHandler = new DataHandler();
        }
        return mDataHandler;
    }

    void filterLogs(const QString &tag, const QString &msg, const QString &level, const QString &pid);
    void filterSettings(const QString nameFilter);
    void filterProperties(const QString nameFilter);

    void loadLogs(const QString &file);
    void loadSettings(const QString deviceId);
    void loadProperties(const QString deviceId);

    void clearLogcat(const QString deviceId);
    void startWatchLog(QString filePath, const QString deviceId);
    void startWatchLogRealTime(const QString deviceId, const bool isWatching);

    QString previousKey(Ui::MainWindow *ui, QObject *obj);
    QString nextKey(Ui::MainWindow *ui, QObject *obj);

    void saveLog(const QString filePath);
};

#endif // DATAHANDLER_H
