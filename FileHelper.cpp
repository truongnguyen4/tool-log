#include "FileHelper.hpp"
#include <QFile>
#include "Logger.hpp"
#include "NotificationHelper.hpp"
#include "QString"
#include "mainwindow.h"
#include "LogHelper.hpp"
#include <QString>
#include <QFileInfo>

const QString FileHelper::TAG = "FileHelper";

void FileHelper::readLogsFromFile(const QString filePath)
{
    Logger::d(TAG, "Reading log file: " + filePath);
    if (!checkPath(filePath))
    {
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            Log log = mLogHelper->convertToLog(line);
            mLogHelper->updateHiddenLog(log);
            mLogHelper->mListObjs.push_back(log);
        }
        file.close();
        return;
    }
}

bool FileHelper::checkPath(const QString filePath)
{
    QString path = filePath.trimmed();
    if (path.isEmpty())
    {
        Logger::e(TAG, "Empty file path");
        NotificationHelper::showError(NotificationHelper::ERROR_FILE_PATH);
        return false;
    }

    QFileInfo info(path);
    // Resolve symlinks / get canonical path when possible for more accurate checks
    QString canonical = info.canonicalFilePath();
    if (!canonical.isEmpty())
    {
        info = QFileInfo(canonical);
        path = info.absoluteFilePath();
    }

    const QStringList allowedExt = { "txt", "log" };
    QString ext = info.suffix().toLower();

    if (info.exists())
    {
        if (!info.isFile())
        {
            Logger::e(TAG, QString("Path exists but is not a regular file: %1").arg(path));
            NotificationHelper::showError(NotificationHelper::ERROR_FILE_PATH);
            return false;
        }

        // Guard against unexpectedly large files being loaded into memory
        constexpr qint64 MAX_READ_SIZE = 50LL * 1024 * 1024; // 50 MB
        if (info.size() > MAX_READ_SIZE)
        {
            Logger::e(TAG, QString("File too large to read safely (%1 bytes): %2").arg(info.size()).arg(path));
            NotificationHelper::showError(NotificationHelper::ERROR_FILE_PATH);
            return false;
        }
    }
    else
    {
        // File doesn't exist -> allow creation only if parent directory exists and is writable.
        QString parentPath = info.absolutePath();
        QFileInfo parentInfo(parentPath);
        if (!parentInfo.exists() || !parentInfo.isDir())
        {
            Logger::e(TAG, QString("Parent directory does not exist: %1").arg(parentPath));
            NotificationHelper::showError(NotificationHelper::ERROR_FILE_PATH);
            return false;
        }

        if (!parentInfo.isWritable())
        {
            Logger::e(TAG, QString("Parent directory is not writable: %1").arg(parentPath));
            NotificationHelper::showError(NotificationHelper::ERROR_FILE_PATH);
            return false;
        }
    }

    // If an extension is present, validate it; allow creation of files without an extension.
    if (!ext.isEmpty() && !allowedExt.contains(ext))
    {
        Logger::e(TAG, QString("Unsupported file extension: %1").arg(ext));
        NotificationHelper::showError(NotificationHelper::ERROR_FILE_PATH);
        return false;
    }

    return true;
}

void FileHelper::saveToFile(const QString filePath, const QStringList lines)
{
    // For saving, allow creating a new file; just ensure the path is not empty.
    if (!checkPath(filePath))
    {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        Logger::e(TAG, QString("Unable to open file for writing: %1").arg(filePath));
        NotificationHelper::showError(NotificationHelper::ERROR_FILE_PATH);
        return;
    }

    QTextStream out(&file);
    for (const QString &line : lines)
    {
        out << line << "\n";
    }
    file.close();
}
