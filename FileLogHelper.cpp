#include "FileLogHelper.hpp"
#include <regex>
#include <fstream>
#include "Logger.hpp"

using std::string;
using std::vector;

const std::regex FileLogHelper::logcatPattern(
        R"(^(\d{2}-\d{2})\s+(\d{2}:\d{2}:\d{2}\.\d+)\s+(\d+)\s+(\d+)\s+([A-Zdiewv])\s+([^:]+):\s+(.*)$)", std::regex::icase);
        
void FileLogHelper::readLog(const string &filePath)
{
    Logger::d("FileLogHelper", "Reading log file: " + filePath);
    mListLogs.clear();
    Log::static_id = 1;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        Logger::w("FileLogHelper", "Failed to open log file: " + filePath);
        return;
    }

    // Reserve space if possible to reduce the number of time of reallocations
    mListLogs.reserve(1024);

    string line;
    while (std::getline(file, line))
    {
        mListLogs.push_back(convertLog(line));
    }
    file.close();
}

Log FileLogHelper::convertLog(const string &line)
{
    std::smatch matches;
    Log log;
    if (std::regex_match(line, matches, logcatPattern))
    {
        log.setDate(matches[1].str());
        log.setTime(matches[2].str());
        log.setPid(matches[3].str());
        log.setTid(matches[4].str());
        log.setLevel(matches[5].str());
        log.setTag(matches[6].str());
        log.setMsg(matches[7].str());
    }
    else
    {
        log.setMsg(line);
    }
    return log;
}

void FileLogHelper::readLogsFromFile(const string &filePath)
{
    mFilePath = filePath;
    Logger::setTimeFrom("ReadFile", steady_clock::now());
    readLog(mFilePath);
    Logger::setTimeTo("ReadFile", steady_clock::now());
}

vector<Log> &FileLogHelper::getListLogs()
{
    return mListLogs;
}

string FileLogHelper::getNameFile()
{
    if (mFilePath.empty())
    {
        Logger::w("FileLogHelper", "File path is empty, returning default name.");
        return "default.log";
    }
    size_t lastSlash = mFilePath.find_last_of("/\\");
    if (lastSlash != string::npos)
    {
        return mFilePath.substr(lastSlash + 1);
    }
    return "default.log";
}

int FileLogHelper::getSizeFile()
{
    return mListLogs.size();
}


bool FileLogHelper::reverseIsMarkLog(int index)
{
    return mListLogs[index].revertIsMarked();
}

Log FileLogHelper::getLog(int index)
{
    return mListLogs[index];
}


