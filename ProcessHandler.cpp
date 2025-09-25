#include "ProcessHandler.hpp"
#include <QProcess>
#include "Logger.hpp"
#include "mainwindow.h"

QString const ProcessHandler::TAG = "ProcessHandler";
QProcess *ProcessHandler::process = new QProcess();
QString ProcessHandler::mDeviceId = "";

QString ProcessHandler::runShellCommand(const QString &program, const QStringList &args)
{
    QProcess *process = new QProcess();
    process->start(program, args);
    process->waitForFinished();

    QString output = process->readAllStandardOutput();
    delete process;

    return output;
}

QStringList ProcessHandler::getDeviceIds()
{
    QStringList deviceIds;
    QString output = runShellCommand("adb", QStringList() << "devices");
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    if (lines.length() > 1)
    {
        for (int i=1; i<lines.length(); i++)
        {
            const QString deviceId = lines[i].split('\t')[0].trimmed();
            if (!deviceId.isEmpty())
            {
                deviceIds.append(deviceId);
            }
        }
    }
    return deviceIds;
}

bool ProcessHandler::checkDeviceId(const QString deviceId)
{
    if (!getDeviceIds().contains(deviceId))
    {
        return false;
    }
    ProcessHandler::mDeviceId = deviceId;
    return true;
}

int ProcessHandler::clearLogcat()
{
    runShellCommand("adb", QStringList() << "-s" << ProcessHandler::mDeviceId << "logcat" << "-c");
    return MainWindow::SUCCESS;
}

bool ProcessHandler::startWatchLog(const QString filePath)
{
    // Check if the file is already being watched, stop the current watch
    if (process->state() == QProcess::Running)
    {
        ProcessHandler::process->kill();
        ProcessHandler::process->waitForFinished();
        Logger::d(TAG, "Stop watching log");
        return MainWindow::SUCCESS;
    }

    ProcessHandler::process->setStandardOutputFile(filePath, QIODevice::NewOnly);
    ProcessHandler::process->start("adb", QStringList() << "-s" << ProcessHandler::mDeviceId << "logcat");
    Logger::d(TAG, "Start watching log, PID = " + QString::number(process->processId()) + ", DEVICE ID = " + ProcessHandler::mDeviceId);

    return MainWindow::SUCCESS;
}
