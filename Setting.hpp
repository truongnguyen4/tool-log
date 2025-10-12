#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>
class Setting
{
private:
    QString group;
    QString name;
    QString value;
public:
    Setting();
    void setGroup(const QString group);
    void setName(const QString name);
    void setValue(const QString value);
    QString getGroup() const;
    QString getName() const;
    QString getValue() const;
    const QString toString() {
        return "group: " + group + ", name: " + name + ", value: " + value;
    }
};

#endif // SETTINGS_H
