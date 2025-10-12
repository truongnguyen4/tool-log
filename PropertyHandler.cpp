#include "PropertyHandler.hpp"
#include "ProcessHelper.hpp"
#include "Setting.hpp"
#include "Property.hpp"
#include "Constant.hpp"

void PropertyHandler::setSettings(const QList<Setting> settings) {
    for (const Setting &setting : settings) {
        setSetting(setting);
    }
}

void PropertyHandler::setSetting(const Setting setting) {
    // adb -s <device id> shell settings put <group> <name> <value>
    QStringList command = {"-s", "<device id>"
                            , "shell", "settings", "put"
                            , setting.getGroup()
                            , setting.getName()
                            , setting.getValue()};
    ProcessHelper::runShellCommand("adb", command);
}

void PropertyHandler::setProperties(const QList<Property> properties) {
    for (const Property &property : properties) {
        setProperty(property);
    }
}
void PropertyHandler::setProperty(const Property property) {
    // adb -s <device id> shell setprop <name> <value>
    Property mutableProperty = property;
    QStringList command = {"-s", "<device id>"
                            , "shell setprop"
                            , mutableProperty.getName()
                            , mutableProperty.getValue()};
    ProcessHelper::runShellCommand("adb", command);
}

QList<Setting> PropertyHandler::loadSettings() {
    QList<Setting> settings;
    for (const QString &group : {Constant::PropertyGroup::Global, Constant::PropertyGroup::System, Constant::PropertyGroup::Secure})
    {
        QList<Setting> groupSettings = loadSettings(group);
        settings.append(groupSettings);
    }
    return settings;
}

QList<Setting> PropertyHandler::loadSettings(const QString group) {
    QList<Setting> settings;
    // adb -s <device id> shell settings list <group>
    QStringList command = {"-s", "<device id>"
                            , "shell settings list"
                            , group};
    QString output = ProcessHelper::runShellCommand("adb", command);
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    // TODO: parse lines to Setting objects
    return settings;
}

QList<Property> PropertyHandler::loadProperties() {
    QList<Property> properties;
    // adb -s <device id> shell getprop
    QStringList command = {"-s", "<device id>"
                            , "shell getprop"};
    QString output = ProcessHelper::runShellCommand("adb", command);
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    // TODO: parse lines to Property objects
    return properties;
}
