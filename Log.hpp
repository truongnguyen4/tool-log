#ifndef LOG_HPP
#define LOG_HPP
#include <string>

using std::string;

class Log
{
private:
    int line = 0;
    string date = "default";
    string time = "default";
    string pid = "default";
    string tid = "default";
    string level = "default";
    string tag = "default";
    string msg = "default";
    bool isHidden = false;
    bool isMarked = false;

public:
    static int static_id;

    Log();

    const int &getLine() const;
    const string &getDate() const;
    const string &getTime() const;
    const string &getPid() const;
    const string &getTid() const;
    const string &getLevel() const;
    const string &getTag() const;
    const string &getMsg() const;
    const bool &getHidden() const;
    const bool &getIsMarked() const;

    void setDate(const string &value);
    void setTime(const string &value);
    void setPid(const string &value);
    void setTid(const string &value);
    void setLevel(const string &value);
    void setTag(const string &value);
    void setMsg(const string &value);
    void setHidden(const bool &value);

    bool revertIsMarked();

    string toString() const;

};

#endif // LOG_HPP
