#include "PropertyHelper.hpp"
#include "ProcessHelper.hpp"
#include <QList>
#include "Logger.hpp"
#include "NotificationHelper.hpp"



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
    int exitCode = 0;
    QString output = ProcessHelper::runShellCommand("adb", command, exitCode);
    if (exitCode != 0)
    {
        NotificationHelper::showExitCode();
        return;
    }
}

void PropertyHelper::loadProperties(const QString deviceId)
{
    Logger::d(TAG, QString("loadProperties: deviceId = %1").arg(deviceId));
    PropertyHelper::mListProperties.clear();
    Property::static_line = 0;

    QStringList command = {"-s", deviceId, "shell", "getprop"};
    int exitCode = 0;
    QString output = ProcessHelper::runShellCommand("adb", command, exitCode);

    if (exitCode != 0)
    {
        Logger::e(TAG, QString("Failed to get properties for device %1, exitCode: %2").arg(deviceId).arg(exitCode));
        NotificationHelper::showExitCode();
        return;
    }

    const QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines)
    {
        Property property = PropertyHelper::convertToProperty(line.trimmed());
        if (!property.getName().isEmpty())
        {
            PropertyHelper::mListProperties.append(property);
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

void PropertyHelper::filterProperty(QList<Property> &properties, const QStringList names)
{
    Logger::d(TAG, "filterProperty, names = " + names.join(", "));

    for (Property &property : properties)
    {
        if (names.empty())
        {
            property.setIsHidden(false);
        }
        else
        {
            bool isHidden = true;
            for (const QString &name : names)
            {
                if (property.getName().contains(name, Qt::CaseInsensitive))
                {
                    isHidden = false;
                    break;
                }
            }
            property.setIsHidden(isHidden);
        }
    }
}
