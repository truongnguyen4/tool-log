#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>
#include <QStringList>
class Setting
{
private:
    int line = 0;
    QString group;
    QString name;
    QString value;
    bool isHidden = false;

public:
    inline static int static_id = 0;
    Setting() : line(++static_id) {};
    Setting(QString group, QString name, QString value) : group(group), name(name), value(value), line(++static_id) {};
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
    bool getHidden() const
    {
        return isHidden;
    }
    void setHidden(const bool isHidden)
    {
        this->isHidden = isHidden;
    }
    int getLine() const
    {
        return line;
    }

    const QString toString()
    {
        return QString("line: %1, group: %2, name: %3, value: %4, isHidden: %5")
            .arg(getLine())
            .arg(group)
            .arg(name)
            .arg(value)
            .arg(isHidden ? QString("true") : QString("false"));
    }
    QStringList getListData() const
    {
        return QStringList{
            QString::number(line),
            group,
            name,
            value};
    }
};

#endif // SETTINGS_H
