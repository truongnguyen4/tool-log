#ifndef LOG_HPP
#define LOG_HPP
#include <QString>
#include <QStringList>

class Log
{
private:
    int line = 0;
    QString date;
    QString time;
    QString pid;
    QString tid;
    QString level;
    QString tag;
    QString msg;
    bool isHidden = false;
    bool isMarked = false;

public:
    static inline int static_id = 0;

    Log() : line(++static_id) {}
    Log(QString msg) : msg(msg), line(++static_id) {}
    Log(QString date, QString time, QString pid, QString tid, QString level, QString tag, QString msg)
        : date(date), time(time), pid(pid), tid(tid), level(level), tag(tag), msg(msg), line(++static_id) {}

    const int &getLine() const
    {
        return line;
    }
    const QString &getDate() const
    {
        return date;
    }
    const QString &getTime() const
    {
        return time;
    }
    const QString &getPid() const
    {
        return pid;
    }
    const QString &getTid() const
    {
        return tid;
    }
    const QString &getLevel() const
    {
        return level;
    }
    const QString &getTag() const
    {
        return tag;
    }
    const QString &getMsg() const
    {
        return msg;
    }
    const bool &getHidden() const
    {
        return isHidden;
    }
    const bool &getIsMarked() const
    {
        return isMarked;
    }
    void setDate(const QString &value)
    {
        date = value;
    }
    void setTime(const QString &value)
    {
        time = value;
    }
    void setPid(const QString &value)
    {
        pid = value;
    }
    void setTid(const QString &value)
    {
        tid = value;
    }
    void setLevel(const QString &value)
    {
        level = value;
    }
    void setTag(const QString &value)
    {
        tag = value;
    }
    void setMsg(const QString &value)
    {
        msg = value;
    }
    void setHidden(const bool &value)
    {
        isHidden = value;
    }
    bool revertIsMarked()
    {
        isMarked = !isMarked;
        return isMarked;
    }

    QString toString() const
    {
        return QString("%1 %2 %3 %4 %5 %6 : %7")
            .arg(date)
            .arg(time)
            .arg(pid)
            .arg(tid)
            .arg(level)
            .arg(tag)
            .arg(msg);
    }

    QStringList getListData() const
    {
        return QStringList{QString::number(line)
                           , date
                           , time
                           , pid
                           , tid
                           , level
                           , tag
                           , msg};
    }
};

#endif // LOG_HPP
