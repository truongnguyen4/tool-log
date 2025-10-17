#ifndef LOGGER_H
#define LOGGER_H

#include "QString"
#include <iostream>
#include <QMap>
#include <QPair>
using std::cout;
using std::endl;
class Logger
{
public:
    static void d(const QString &tag, const QString &msg) 
    {
        if (Logger::VERBOSE)
        {
            std::cout << "[DEBUG] [" << tag.toStdString() << "] " << msg.toStdString() << std::endl;
        }
    }
    static void w(const QString &tag, const QString &msg)
    {
        if (Logger::VERBOSE)
        {
            std::cout << "[WARNING] [" << tag.toStdString() << "] " << msg.toStdString() << std::endl;
        }
    }
    static void e(const QString &tag, const QString &msg)
    {
        if (Logger::VERBOSE)
        {
            std::cout << "[ERROR] [" << tag.toStdString() << "] " << msg.toStdString() << std::endl;
        }
    }

    static void setTimeFrom(const QString &name, qint64 timeFrom)
    {
        if (Logger::VERBOSE)
        {
            mTimeStampMap[name] = qMakePair(timeFrom, qint64());
        }
    }

    static void setTimeTo(const QString &name, qint64 timeTo)
    {
        if (Logger::VERBOSE)
        {
            if (mTimeStampMap.keys().contains(name))
            {
                mTimeStampMap[name].second = timeTo;
                if (Logger::VERBOSE)
                {
                    d("Timer", "Time for " + name + ": " + QString::number(mTimeStampMap[name].second - mTimeStampMap[name].first) + " ms");
                }
                mTimeStampMap.remove(name);
            }
            else
            {
                w("Timer", "No start time for " + name + " found.");
            }
        }
    }
private:
    inline static QMap<QString, QPair<qint64, qint64>> mTimeStampMap = QMap<QString, QPair<qint64, qint64>>();
    inline static bool VERBOSE = true;

};
#endif // LOGGER_H
