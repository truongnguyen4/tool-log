#ifndef LOG_HPP
#define LOG_HPP
#include <QString>

class Log
{
private:
    int line = 0;
    QString date = "default";
    QString time = "default";
    QString pid = "default";
    QString tid = "default";
    QString level = "default";
    QString tag = "default";
    QString msg = "default";
    bool isHidden = false;
    bool isMarked = false;

public:
    static int static_id;

    Log();

    const int &getLine() const;
    const QString &getDate() const;
    const QString &getTime() const;
    const QString &getPid() const;
    const QString &getTid() const;
    const QString &getLevel() const;
    const QString &getTag() const;
    const QString &getMsg() const;
    const bool &getHidden() const;
    const bool &getIsMarked() const;

    void setDate(const QString &value);
    void setTime(const QString &value);
    void setPid(const QString &value);
    void setTid(const QString &value);
    void setLevel(const QString &value);
    void setTag(const QString &value);
    void setMsg(const QString &value);
    void setHidden(const bool &value);

    bool revertIsMarked();

    QString toString() const;

};

#endif // LOG_HPP
