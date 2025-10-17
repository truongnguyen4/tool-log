#ifndef DATAHANDLER_H
#define DATAHANDLER_H
#include "FileHelper.hpp"
#include "ui_mainwindow.h"
#include <QString>
#include "ProcessHelper.hpp"
#include "Property.hpp"
#include "Setting.hpp"
#include "LogHelper.hpp"
class DataHandler
{
private:
    static const QString TAG;
    FileHelper mFileLogHelper;
    ProcessHelper *mProcessHandler = ProcessHelper::getInstance();
    LogHelper mLogHelper;
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
    static inline DataHandler* mDataHandler = nullptr;
    DataHandler() {};
public:
    static DataHandler* getInstance()
    {
        if (!mDataHandler)
        {
            mDataHandler = new DataHandler();
        }
        return mDataHandler;
    }
    void addKey(const QString &tag, const QString &msg, const QString &level, const QString &pid);
    void addKey(const QString &find);
    void addKey(const QString &property,const QString &setting);

    void filterLogs(const QString &tag, const QString &msg, const QString &level, const QString &pid);
    void filterSettings(const QString nameFilter);
    void filterProperties(const QString nameFilter);

    void refreshLog(const QString &file);
    void loadProperties(const QString deviceId);
    void loadSettings(const QString deviceId);

    void startWatchLog(QString filePath, const QString deviceId);
    void startWatchLogRealTime(const QString deviceId);
    void clearLogcat(const QString deviceId);
    QString previousKey(Ui::MainWindow *ui, QObject *obj);
    QString nextKey(Ui::MainWindow *ui, QObject *obj);
};

#endif // DATAHANDLER_H
