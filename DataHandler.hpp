#ifndef DATAHANDLER_H
#define DATAHANDLER_H
#include "FileHelper.hpp"
#include "ui_mainwindow.h"
#include <QString>
#include "ProcessHelper.hpp"

class DataHandler
{
private:
    static const QString TAG;
    FileHelper mFileLogHelper;
    ProcessHelper mProcessHandler;
    QStringList mListFind;
    QStringList mListMsg;
    QStringList mListTag;
    QStringList mListPid;
    QStringList mListLevel;
    int mFindId = 0;
    int mMsgId = 0;
    int mTagId = 0;
    int mPidId = 0;
    int mLevelId = 0;
public:
    void addKey(const QString &tag, const QString &msg, const QString &level, const QString &pid);
    void addKey(const QString &find);
    QList<Log> onFilterKeyChanged(const QString &tag, const QString &msg, const QString &level, const QString &pid);
    QList<Log> refreshLog(const QString &file);
    int startWatchLog(QString filePath, const QString deviceId);
    int clearLogcat(const QString deviceId);
    QString previousKey(Ui::MainWindow *ui, QObject *obj);
    QString nextKey(Ui::MainWindow *ui, QObject *obj);
};

#endif // DATAHANDLER_H
