#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>
class Setting
{
private:
    int line = 0;
    QString group;
    QString name;
    QString value;
    bool isHidden = false;
public:
    Setting() : line(++static_line) {};
    Setting(QString group, QString name, QString value) : group(group), name(name), value(value), line(++static_line) {};
    inline static int static_line = 0;
    void setGroup(const QString group)
    {
        this->group = group;
    }
    void setName(const QString name)
    {
        this->name = name;
    }
    void setValue(const QString value)
    {
        this->value = value;
    }
    QString getGroup() const
    {
        return group;
    }
    QString getName() const
    {
        return name;
    }
    QString getValue() const
    {
        return value;
    }
    bool getIsHidden() const
    {
        return isHidden;
    }
    void setIsHidden(const bool isHidden)
    {
        this->isHidden = isHidden;
    }
    int getLine() const
    {
        return line;
    }

    const QString toString() {
        return "line: " + QString::number(getLine()) + ", group: " + group + ", name: " + name + ", value: " + value + ", isHidden: " + QString::number(isHidden);
    }
};

#endif // SETTINGS_H
