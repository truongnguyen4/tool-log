#ifndef PROPERTYHELPER_H
#define PROPERTYHELPER_H
#include "Property.hpp"
#include <QList>
#include <QString>
#include <QRegularExpression>

class PropertyHelper
{
private:
    inline static const QString TAG = "PropertyHelper";
    static const QRegularExpression regexProperty;

public:
    inline static QList<Property> mProperties = QList<Property>();;
    static void loadProperties(const QString deviceId);
    static void setProperties(const QList<Property> properties, const QString deviceId);
    static void setProperty(const Property property, const QString deviceId);
    static Property convertToProperty(const QString line);
    static QList<Property> filterProperty(QList<Property> properties, const QStringList name);
};


#endif // PROPERTYHELPER_H
