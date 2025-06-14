#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>
#include <chrono>
#include <utility>
using std::string;
using std::map;
using std::pair;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::make_pair;
using std::chrono::steady_clock;

class Logger
{
public:
    static void d(const string &tag, const string &msg);
    static void w(const string &tag, const string &msg);
    static void e(const string &tag, const string &msg);

    static void setTimeFrom(const string &name, steady_clock::time_point timeFrom);
    static void setTimeTo(const string &name, steady_clock::time_point timeTo);
    static bool VERBOSE;
private:
    static map<string, pair<steady_clock::time_point, steady_clock::time_point>> timestampMap;
};
#endif // LOGGER_H
