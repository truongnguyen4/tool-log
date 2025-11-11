#include "PropertyHelper.hpp"
#include "ProcessHelper.hpp"
#include <QList>
#include "Logger.hpp"
#include "UtilHelper.hpp"

void PropertyHelper::setProperties(const QList<Property> properties, const QString deviceId)
{
    for (const Property &property : properties)
    {
        setProperty(property, deviceId);
    }
}

void PropertyHelper::setProperty(const Property property, const QString deviceId)
{
    QStringList command = {"-s", deviceId, "shell", "setprop", property.getName(), property.getValue()};
    QString output = mProcessHelper->runShellCommand("adb", command);
}

void PropertyHelper::loadProperties(const QString deviceId)
{
    Logger::d(TAG, QString("loadProperties: deviceId = %1").arg(deviceId));

    QStringList command = {"-s", deviceId, "shell", "getprop"};
    QString output = mProcessHelper->runShellCommand("adb", command);
    const QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines)
    {
        Property property = PropertyHelper::convertToProperty(line.trimmed());
        if (!property.getName().isEmpty())
        {
            PropertyHelper::getInstance()->mListObjs.append(property);
        }
    }
}

Property PropertyHelper::convertToProperty(const QString line)
{
    QRegularExpressionMatch match = regexProperty.match(line);
    if (match.hasMatch())
    {
        return Property(match.captured(1), match.captured(2));
    }
    Logger::w(TAG, "Can't convert property, line: " + line);
    return Property();
}

void PropertyHelper::filterProperties()
{
    Logger::d(TAG, "filterProperty, mProps = " + mListKeys.join(", "));
    for (Property &property : mListObjs)
    {
        property.setHidden(false);
        mUtilHelper->updateHidden<Property>(property, property.getName(), mListKeys, andOp);
        if (property.getName() == "persist.sys.dalvik.vm.lib.2")
        {
            Logger::d(TAG, "property = " + property.toString());
        }
    }
}

