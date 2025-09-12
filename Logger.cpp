#include "Logger.hpp"
#include <iostream>
#include <chrono>

using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::chrono::steady_clock;

map<string, pair<steady_clock::time_point, steady_clock::time_point>> Logger::timestampMap;
bool Logger::VERBOSE = true;

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

void Logger::setTimeFrom(const QString &name, steady_clock::time_point timeFrom)
{
    if (VERBOSE)
    {
        timestampMap[name.toStdString()] = make_pair(timeFrom, steady_clock::time_point());
    }
}

void Logger::setTimeTo(const QString &name, steady_clock::time_point timeTo)
{
    if (VERBOSE)
    {
        auto it = timestampMap.find(name.toStdString());
        if (it != timestampMap.end())
        {
            it->second.second = timeTo;
            if (VERBOSE)
            {
                auto duration = duration_cast<milliseconds>(it->second.second - it->second.first).count();
                d("Timer", QString::fromStdString("Time for " + name.toStdString() + ": " + std::to_string(duration) + " ms"));
            }
            timestampMap.erase(it);
        }
        else
        {
            w("Timer", QString::fromStdString("Timestamp for " + name.toStdString() + " not found."));
        }
    }
}
