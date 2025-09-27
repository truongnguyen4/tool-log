#ifndef DATAHANDLER_H
#define DATAHANDLER_H
#include "FileHelper.hpp"
#include "FilterHelper.hpp"
#include "./ui_mainwindow.h"
#include <QString>
#include <vector>
#include "ProcessHelper.hpp"
using std::vector;
using std::string;

class DataHandler
{
private:
    static const QString TAG;
    FileHelper mFileLogHelper;
    FilterHelper mFilterLogHelper;
    ProcessHelper mProcessHandler;
    vector<QString> mListMsg;
    vector<QString> mListTag;
    vector<QString> mListPid;
    vector<QString> mListLevel;
    int mMsgId = 0;
    int mTagId = 0;
    int mPidId = 0;
    int mLevelId = 0;
    void addKey(const QString &tag, const QString &msg, const QString &level, const QString &pid);
public:
    QList<Log> &onFilterKeyChanged(const QString &tag, const QString &msg, const QString &level, const QString &pid);
    FileHelper& getFileLogHelper();
    QList<Log> refreshLog(const QString &file);
    int startWatchLog(QString filePath, const QString deviceId);
    int clearLogcat(const QString deviceId);
    QString previousKey(Ui::MainWindow *ui, QObject *obj);
    QString nextKey(Ui::MainWindow *ui, QObject *obj);
    QStringList getDeviceIds();
    void deviceIdChanged(const QString deviceId);
};


#endif // DATAHANDLER_H
