#ifndef PROPERTY_H
#define PROPERTY_H
#include <QString>
#include <QStringList>

class Property
{
private:
    int line = 0;
    QString name;
    QString value;
    bool isHidden = false;

public:
    Property() : line(++static_id) {};
    Property(QString name, QString value) : name(name), value(value), line(++static_id) {};
    inline static int static_id = 0;
    void setName(const QString name)
    {
        this->name = name;
    }
    void setValue(const QString value)
    {
        this->value = value;
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
    QString toString()
    {
        return QStringLiteral("line: %1, name: %2, value: %3, isHidden: %4")
            .arg(getLine())
            .arg(name)
            .arg(value)
            .arg(isHidden ? QStringLiteral("true") : QStringLiteral("false"));
    }
    QStringList getListData() const
    {
        return QStringList{
            QString::number(line),
            name,
            value};
    }
};

#endif // PROPERTY_H
