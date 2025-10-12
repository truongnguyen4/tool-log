#ifndef PROPERTYHANDLER_H
#define PROPERTYHANDLER_H
#include <QString>
#include "Property.hpp"
#include "Setting.hpp"

class PropertyHandler {
public:
    void setSettings(const QList<Setting> settings);
    void setSetting(const Setting setting);
    void setProperties(const QList<Property> properties);
    void setProperty(const Property property);
    QList<Setting> loadSettings(const QString group);
    QList<Setting> loadSettings();
    QList<Property> loadProperties();
};

#endif // PROPERTYHANDLER_H
