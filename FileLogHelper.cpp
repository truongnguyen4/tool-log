#include "FileLogHelper.hpp"
#include <regex>
#include <fstream>
#include <QFile>
#include "Logger.hpp"
#include "NotificationDialog.hpp"
#include "QString"

using std::string;
using std::vector;

const QString FileLogHelper::TAG = "FileLogHelper";
std::atomic_bool FileLogHelper::isWatchingLog = false;

const std::regex FileLogHelper::logcatPattern(
    R"(^(\d{2}-\d{2})\s+(\d{2}:\d{2}:\d{2}\.\d+)\s+(\d+)\s+(\d+)\s+([A-Zdiewv])\s+([^:]+):\s+(.*)$)", std::regex::icase);

void FileLogHelper::readLog(const QString &filePath)
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

bool FileLogHelper::setFilePath(const QString &filePath)
{
    if (checkPath(filePath.toStdString()))
    {
        mFilePath = filePath;
        return true;
    }
    return false;
}

QList<Log> FileLogHelper::readLogsFromFile(const QString &filePath)
{
    readLog(mFilePath);
    return mListLogs;
}

QList<Log> &FileLogHelper::getListLogs()
{
    return mListLogs;
}

string FileLogHelper::getNameFile()
{
    if (mFilePath.isEmpty())
    {
        NotificationDialog::show(NotificationDialog::WARNING, "File path is empty, returning default name.", TAG);
        return "default.log";
    }
    size_t lastSlash = mFilePath.toStdString().find_last_of("/\\");
    if (lastSlash != string::npos)
    {
        return mFilePath.toStdString().substr(lastSlash + 1);
    }
    NotificationDialog::show(NotificationDialog::WARNING, "File path does not contain a valid file name, returning default name.", TAG);
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

bool FileLogHelper::startWatchLog(QProcess *& process)
{
    if (checkDevice().isEmpty())
    {
        NotificationDialog::show(NotificationDialog::ERROR, "Can't find any devices", TAG);
        return false;
    }

    // Check if the file is already being watched, stop the current watch
    if (process && process->state() == QProcess::Running)
    {
        Logger::d(TAG, "Stopping current log watch");
        // TODO: save output to file before killing
        process->kill();
        process->waitForFinished();
        Logger::d(TAG, "Current log watch stopped");
        return false;
    }

    Logger::d(TAG, "Starting log watch");



    process->setStandardOutputFile(mFilePath, QIODevice::Append);
    process->start("adb", QStringList() << "logcat");

    return true;
}

QString FileLogHelper::checkDevice()
{
    QString output = runShellCommand(QStringList() << "devices");
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    if (lines.length() == 2)
    {
        if (lines[1].contains("device"))
        {
            QString deviceId = lines[1].split('\t')[0];
            Logger::d(TAG, "Found device: " + deviceId);
            return deviceId;
        }
    }
    return "";
}

bool FileLogHelper::checkPath(const string &filePath)
{
    if (filePath.empty())
    {
        NotificationDialog::show(NotificationDialog::WARNING, "File path is empty, cannot start watching log.", TAG);
        return false;
    }
    // Validate the file path format
    std::vector<std::string> parts;
    size_t start = 0, end;
    while ((end = filePath.find('.', start)) != std::string::npos)
    {
        parts.push_back(filePath.substr(start, end - start));
        start = end + 1;
    }
    parts.push_back(filePath.substr(start));

    if (parts.size() < 2)
    {
        NotificationDialog::show(NotificationDialog::WARNING, "Invalid file path format, must contain at least one dot before the extension.", TAG);
        return false;
    }
    if (parts.back() != "log")
    {
        NotificationDialog::show(NotificationDialog::WARNING, "File is not a log file, must end with '.log'.", TAG);
        return false;
    }

    // Check if the file exists, if not create it
    std::ifstream infile(filePath);
    if (!infile.good())
    {
        std::ofstream outfile(filePath);
        if (!outfile)
        {
            NotificationDialog::show(NotificationDialog::ERROR, "Failed to create log file: " + QString::fromStdString(filePath), TAG);
            return false;
        }
        Logger::d(TAG, QString::fromStdString("Log file created: " + filePath));
        outfile.close();
    }
    return true;
}

void FileLogHelper::clearLogs()
{
    mListLogs.clear();
    QProcess *newProcess = new QProcess();
    newProcess->start("echo", QStringList() << " " << " > " + mFilePath);
    newProcess->waitForFinished();
    delete newProcess;
    newProcess = nullptr;

    if (!checkDevice().isEmpty())
    {
        runShellCommand(QStringList() << "logcat" << "-c");
    }
}

QString FileLogHelper::runShellCommand(const QStringList &args)
{
    Logger::d(TAG, QString::fromStdString("Running shell command: adb " + args.join(" ").toStdString()));

    QProcess *process = new QProcess();
    process->start("adb", args);
    process->waitForFinished();

    QString output = process->readAllStandardOutput();
    delete process;

    Logger::d(TAG, "Finished running shell command");
    return output;
}



