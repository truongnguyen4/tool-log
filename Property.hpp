#ifndef PROPERTY_H
#define PROPERTY_H
#include <QString>
class Property
{
private:
    QString name;
    QString value;
public:
    Property(/* args */);
    void setName(const QString name);
    void setValue(const QString value);
    QString getName() const;
    QString getValue() const;
    QString toString() {
        return "name: " + name + ", value: " + value;
    }
};

#endif // PROPERTY_H
