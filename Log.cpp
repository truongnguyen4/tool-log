#include "Log.hpp"

int Log::static_id = 1;

const int &Log::getLine() const { return line; }

const QString &Log::getDate() const { return date; }
const QString &Log::getTime() const { return time; }
const QString &Log::getPid() const { return pid; }
const QString &Log::getTid() const { return tid; }
const QString &Log::getLevel() const { return level; }
const QString &Log::getTag() const { return tag; }
const QString &Log::getMsg() const { return msg; }
const bool &Log::getHidden() const { return isHidden; }
const bool &Log::getIsMarked() const { return isMarked; }

void Log::setDate(const QString &d) { date = d; }
void Log::setTime(const QString &t) { time = t; }
void Log::setPid(const QString &p) { pid = p; }
void Log::setTid(const QString &t) { tid = t; }
void Log::setLevel(const QString &l) { level = l; }
void Log::setTag(const QString &t) { tag = t; }
void Log::setMsg(const QString &m) { msg = m; }
void Log::setHidden(const bool &d) { isHidden = d; }
bool Log::revertIsMarked()
{
    isMarked = !isMarked;
    return isMarked;
}

QString Log::toString() const
{
    return  QString::number(line) +
           "  " + date +
           "  " + time +
           "  " + pid +
           "  " + tid +
           "  " + level +
           "  " + tag +
           "  " + msg +
           "  " + QString::number(isMarked);
}
