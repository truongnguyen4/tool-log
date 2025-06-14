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

void Logger::d(const string &tag, const string &msg)
{
    if (VERBOSE)
    {
        cout << "[DEBUG] [" << tag << "] " << msg << endl;
    }
}

void Logger::w(const string &tag, const string &msg)
{
    if (VERBOSE)
    {
        cout << "[WARNING] [" << tag << "] " << msg << endl;
    }
}

void Logger::e(const string &tag, const string &msg)
{
    if (VERBOSE)
    {
        cout << "[ERROR] [" << tag << "] " << msg << endl;
    }
}

void Logger::setTimeFrom(const string &name, steady_clock::time_point timeFrom)
{
    timestampMap[name] = make_pair(timeFrom, steady_clock::time_point());
}

void Logger::setTimeTo(const string &name, steady_clock::time_point timeTo)
{
    auto it = timestampMap.find(name);
    if (it != timestampMap.end())
    {
        it->second.second = timeTo;
        if (VERBOSE)
        {
            auto duration = duration_cast<milliseconds>(it->second.second - it->second.first).count();
            d("Timer", "Time for " + name + ": " + std::to_string(duration) + " ms");
        }
        timestampMap.erase(it);
    }
    else
    {
        w("Logger", "Timestamp for " + name + " not found.");
    }
}
