#ifndef FILELOGHELPER_H
#define FILELOGHELPER_H
#include "Log.hpp"
#include <QProcess>
#include <QMutex>
#include <QRegularExpression>

class FileHelper : public QObject
{
    Q_OBJECT
private:
    static const QString TAG;
public:
    static bool checkPath(const QString filePath);
    QList<Log> readLogsFromFile(const QString filePath);
};
#endif // FILELOGHELPER_H
