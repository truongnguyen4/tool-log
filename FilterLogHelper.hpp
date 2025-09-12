#ifndef FILTERLOGHELPER_H
#define FILTERLOGHELPER_H
#include <vector>
#include <string>
#include <regex>
#include "Log.hpp"
#include "FileLogHelper.hpp"
#include "Logger.hpp"

using std::string;
using std::vector;
using std::regex;
using std::smatch;

class FilterLogHelper
{
private:
    static const QString TAG;
public:
    QList<Log> &filterLogs(QList<Log> &logs, int from, int to, string pid, string tag, string msg, string level);
};

#endif // FILTERLOGHELPER_H
