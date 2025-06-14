#include "DataHandler.hpp"

void DataHandler::reloadData()
{

}

vector<Log> &DataHandler::onFilterKeyChanged(const QString &pid, const QString &tag, const QString &msg, const QString &level)
{
    return mFilterLogHelper.filterLogs(mFileLogHelper.getListLogs(), 1, mFileLogHelper.getSizeFile(), pid.toStdString(), tag.toStdString(), msg.toStdString(), level.toStdString());

}

FileLogHelper &DataHandler::getFileLogHelper()
{
    return mFileLogHelper;
}

vector<Log> DataHandler::refreshLog(const QString &file)
{
    mFileLogHelper.readLogsFromFile(file.toStdString());
    return mFileLogHelper.getListLogs();
}
