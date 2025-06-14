#ifndef DATAHANDLER_H
#define DATAHANDLER_H
#include "FileLogHelper.hpp"
#include "FilterLogHelper.hpp"
#include "./ui_mainwindow.h"
#include <QString>
#include <vector>
using std::vector;
using std::string;

class DataHandler
{
private:
    FileLogHelper mFileLogHelper;
    FilterLogHelper mFilterLogHelper;
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
    vector<Log> &onFilterKeyChanged(const QString &tag, const QString &msg, const QString &level, const QString &pid);
    FileLogHelper& getFileLogHelper();
    vector<Log> refreshLog(const QString &file);
    bool startWatchLog(QString file);
    void clearLogs();
    QString previousKey(Ui::MainWindow *ui, QObject *obj);
    QString nextKey(Ui::MainWindow *ui, QObject *obj);
};


#endif // DATAHANDLER_H
