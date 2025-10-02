#include "Logger.hpp"
#include <iostream>
#include <QMap>
#include <QPair>

using std::cout;
using std::endl;

QMap<QString, QPair<qint64, qint64>> Logger::mTimeStampMap = QMap<QString, QPair<qint64, qint64>>();

bool Logger::VERBOSE = false;

void Logger::d(const QString &tag, const QString &msg)
{
    if (VERBOSE)
    {
        cout << "[DEBUG] [" << tag.toStdString() << "] " << msg.toStdString() << endl;
    }
}

void Logger::w(const QString &tag, const QString &msg)
{
    if (VERBOSE)
    {
        cout << "[WARNING] [" << tag.toStdString() << "] " << msg.toStdString() << endl;
    }
}

void Logger::e(const QString &tag, const QString &msg)
{
    if (VERBOSE)
    {
        cout << "[ERROR] [" << tag.toStdString() << "] " << msg.toStdString() << endl;
    }
}

void Logger::setTimeFrom(const QString &name, qint64 timeFrom)
{
    if (VERBOSE)
    {
        mTimeStampMap[name] = qMakePair(timeFrom, qint64());
    }
}

void Logger::setTimeTo(const QString &name, qint64 timeTo)
{
    if (VERBOSE)
    {
        if (mTimeStampMap.keys().contains(name))
        {
            mTimeStampMap[name].second = timeTo;
            if (VERBOSE)
            {

                d("Timer", "Time for " + name + ": " + QString::number(mTimeStampMap[name].second - mTimeStampMap[name].first) + " ms");
            }
            mTimeStampMap.remove(name);
        }
        else
        {
            w("Timer", "Timestamp for " + name + " not found.");
        }
    }
}
