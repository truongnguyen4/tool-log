#include "FileHelper.hpp"
#include <QFile>
#include "Logger.hpp"
#include "NotificationHelper.hpp"
#include "QString"
#include "mainwindow.h"
#include "LogHelper.hpp"
#include <QString>

const QString FileHelper::TAG = "FileHelper";

QList<Log> FileHelper::readLogsFromFile(const QString filePath)
{
    Logger::d(TAG, "Reading log file: " + filePath);
    if (!checkPath(filePath))
    {
        Logger::e(TAG, "Invalid file path: " + filePath);
        NotificationHelper::showError(MainWindow::ERROR_FILE_PATH);
        return QList<Log>();
    }

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QList<Log> logs;
        Log::static_id = 1;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            Log log = LogHelper::convertToLog(line);
            LogHelper::getInstance()->updateHiddenLog(log);
            logs.push_back(log);
        }
        file.close();
        return logs;
    }

    Logger::e(TAG, "Failed to open file: " + filePath);
    NotificationHelper::showError(MainWindow::ERROR_FILE_PATH);
    return QList<Log>();
}

bool FileHelper::checkPath(const QString filePath)
{
    if (filePath.isEmpty())
    {
        Logger::e(TAG, "Empty file path");
        NotificationHelper::showError(MainWindow::ERROR_FILE_PATH);
        return false;
    }
    // Validate the file path format
    QStringList parts = filePath.split(".");
    QString extension = parts.last();
    if (extension != "txt" && extension != "log")
    {
        Logger::e(TAG, "Invalid file extension: " + extension);
        NotificationHelper::showError(MainWindow::ERROR_FILE_PATH);
        return false;
    }
    QFile file(filePath);
    if (!file.exists())
    {
        Logger::e(TAG, "File does not exist: " + filePath);
        NotificationHelper::showError(MainWindow::ERROR_FILE_PATH);
        return false;
    }
    return true;
}

