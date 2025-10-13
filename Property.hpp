#ifndef PROPERTY_H
#define PROPERTY_H
#include <QString>
class Property
{
private:
int line = 0;
    QString name = "default";
    QString value = "default";
    bool isHidden = false;
public:
    Property() : line(++static_line) {};
    Property(QString name, QString value) : name(name), value(value), line(++static_line) {};
    inline static int static_line = 0;
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
    QString toString() {
        return "line: " + QString::number(getLine()) + ", name: " + name + ", value: " + value + ", isHidden: " + QString::number(isHidden);
    }
};

#endif // PROPERTY_H
