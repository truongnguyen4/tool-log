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
    const QRegularExpression regexProperty = QRegularExpression(
        R"(\[([^\]]+)\]: \[([^\]]*)\])",
        QRegularExpression::CaseInsensitiveOption);

    static inline PropertyHelper *instance = nullptr;
    PropertyHelper() {};

public:
    static PropertyHelper *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new PropertyHelper();
        }
        return instance;
    }
    QList<Property> mListProperties;
    void filterProperty(QList<Property> &properties, const QStringList name);
    void loadProperties(const QString deviceId);
    void setProperties(const QList<Property> properties, const QString deviceId);
    void setProperty(const Property property, const QString deviceId);
    Property convertToProperty(const QString line);
};

#endif // PROPERTYHELPER_H
