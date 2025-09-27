#include "FileHelper.hpp"
#include <regex>
#include <fstream>
#include <QFile>
#include "Logger.hpp"
#include "NotificationDialog.hpp"
#include "QString"

using std::string;
using std::vector;

const QString FileHelper::TAG = "FileHelper";
QString FileHelper::mFilePath = "";

const std::regex FileHelper::logcatPattern(
    R"(^(\d{2}-\d{2})\s+(\d{2}:\d{2}:\d{2}\.\d+)\s+(\d+)\s+(\d+)\s+([A-Zdiewv])\s+([^:]+):\s+(.*)$)", std::regex::icase);

void FileHelper::readLog(const QString &filePath)
{
    Logger::d(TAG, "Reading log file: " + filePath);

    mListLogs.clear();
    Log::static_id = 1;

    std::ifstream file(filePath.toStdString());
    if (!file.is_open())
    {
        NotificationDialog::show(NotificationDialog::WARNING, "Failed to open log file: " + filePath, TAG);
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

Log FileHelper::convertLog(const string &line)
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

void FileHelper::setFilePath(const QString &filePath)
{
    mFilePath = filePath;
}

QList<Log> FileHelper::readLogsFromFile()
{
    QMutexLocker locker(&mtxListLog);
    readLog(mFilePath);
    return mListLogs;
}

QList<Log> &FileHelper::getListLogs()
{
    QMutexLocker locker(&mtxListLog);
    return mListLogs;
}

QString FileHelper::getNameFile()
{
    if (mFilePath.isEmpty())
    {
        NotificationDialog::show(NotificationDialog::WARNING, "File path is empty, returning default name.", TAG);
        return "default.log";
    }
    size_t lastSlash = mFilePath.toStdString().find_last_of("/\\");
    if (lastSlash != string::npos)
    {
        QString fileName = mFilePath.mid(static_cast<int>(lastSlash) + 1);
        if (!fileName.isEmpty())
            return fileName;
    }
    NotificationDialog::show(NotificationDialog::WARNING, "File path does not contain a valid file name, returning default name.", TAG);
    return "default.log";
}

int FileHelper::getSizeFile()
{
    QMutexLocker locker(&mtxListLog);
    return mListLogs.size();
}

bool FileHelper::reverseIsMarkLog(int index)
{
    QMutexLocker locker(&mtxListLog);
    return mListLogs[index].revertIsMarked();
}

Log FileHelper::getLog(int index)
{
    QMutexLocker locker(&mtxListLog);
    return mListLogs[index];
}

bool FileHelper::checkPath(const QString &filePath)
{
    std::string filePathStr = filePath.toStdString();
    if (filePathStr.empty())
    {
        return false;
    }
    // Validate the file path format
    std::vector<std::string> parts;
    size_t start = 0, end;
    while ((end = filePathStr.find('.', start)) != std::string::npos)
    {
        parts.push_back(filePathStr.substr(start, end - start));
        start = end + 1;
    }
    parts.push_back(filePathStr.substr(start));

    if (parts.size() < 2)
    {
        return false;
    }
    if (parts.back() != "log")
    {
        return false;
    }

    // Check if the file exists, if not create it
    std::ifstream infile(filePathStr);
    if (!infile.good())
    {
        std::ofstream outfile(filePathStr);
        if (!outfile)
        {
            outfile.close();
            return false;
        }
        Logger::d(TAG, "Log file created: " + filePath);
        outfile.close();
    }
    FileHelper::mFilePath = filePath;
    return true;
}

