#ifndef FILELOGHELPER_H
#define FILELOGHELPER_H
#include "Log.hpp"
#include <QProcess>
#include <QMutex>
#include <QRegularExpression>
#include "UtilHelper.hpp"
#include "LogHelper.hpp"
class FileHelper : public QObject
{
    Q_OBJECT
private:
    static const QString TAG;
    static inline FileHelper *instance = nullptr;
    FileHelper() {};
    UtilHelper* mUtilHelper = UtilHelper::getInstance();
    LogHelper* mLogHelper = LogHelper::getInstance();
public:
    static FileHelper *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new FileHelper();
        }
        return instance;
    }
    bool checkPath(const QString filePath);
    void readLogsFromFile(const QString filePath);
    void saveToFile(const QString filePath, const QStringList logs);
};
#endif // FILELOGHELPER_H
