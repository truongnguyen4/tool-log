#ifndef DATAHANDLER_H
#define DATAHANDLER_H
#include "FileLogHelper.hpp"
#include "FilterLogHelper.hpp"

#include <QString>
class DataHandler
{
private:
    FileLogHelper mFileLogHelper;
    FilterLogHelper mFilterLogHelper;
    void reloadData();

public:
    vector<Log> &onFilterKeyChanged(const QString &tag, const QString &msg, const QString &level, const QString &pid);
    FileLogHelper& getFileLogHelper();
    vector<Log> refreshLog(const QString &file);
};


#endif // DATAHANDLER_H
