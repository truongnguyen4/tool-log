#ifndef FILELOGHELPER_H
#define FILELOGHELPER_H
#include <vector>
#include "Log.hpp"
#include <regex>

using std::string;
using std::vector;
class FileLogHelper
{
private:
    static const std::regex logcatPattern;
    vector<Log> mListLogs;
    string mFilePath;
    void readLog(const string &filePath);
    Log convertLog(const string &line);
public:
    void readLogsFromFile(const string &filePath);
    vector<Log>& getListLogs();
    string getNameFile();
    int getSizeFile();
    bool reverseIsMarkLog(int line);
    Log getLog(int index);
};
#endif // FILELOGHELPER_H
