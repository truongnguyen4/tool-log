#include "Log.hpp"
#include "Logger.hpp"

int Log::static_id = 1;

Log::Log() : line(static_id++) {}

const int &Log::getLine() const { return line; }

const string &Log::getDate() const { return date; }
const string &Log::getTime() const { return time; }
const string &Log::getPid() const { return pid; }
const string &Log::getTid() const { return tid; }
const string &Log::getLevel() const { return level; }
const string &Log::getTag() const { return tag; }
const string &Log::getMsg() const { return msg; }
const bool &Log::getHidden() const { return isHidden; }
const bool &Log::getIsMarked() const { return isMarked; }

void Log::setDate(const string &d) { date = d; }
void Log::setTime(const string &t) { time = t; }
void Log::setPid(const string &p) { pid = p; }
void Log::setTid(const string &t) { tid = t; }
void Log::setLevel(const string &l) { level = l; }
void Log::setTag(const string &t) { tag = t; }
void Log::setMsg(const string &m) { msg = m; }
void Log::setHidden(const bool &d) { isHidden = d; }
bool Log::revertIsMarked()
{
    isMarked = !isMarked;
    return isMarked;
}

string Log::toString() const
{
    return  std::to_string(line) +
           "  " + date +
           "  " + time +
           "  " + pid +
           "  " + tid +
           "  " + level +
           "  " + tag +
           "  " + msg +
           "  " + std::to_string(isMarked);
}
