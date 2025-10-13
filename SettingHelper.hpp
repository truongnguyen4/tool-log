#ifndef SETTINGHELPER_H
#define SETTINGHELPER_H
#include "Setting.hpp"
#include <QList>
#include <QString>
#include <QRegularExpression>
class SettingHelper
{
private:
    inline static const QString TAG = "SettingHelper";
    static const QRegularExpression regexSetting;

public:
    inline static QList<Setting> mSettings = QList<Setting>();
    static void loadSettings(const QString deviceId);
    static Setting convertToSetting(const QString group, const QString line);
    static void setSettings(const QList<Setting> settings, const QString deviceId);
    static void setSetting(const Setting setting, const QString deviceId);
    static QList<Setting> filterSettings(QList<Setting> settings, const QStringList nameFilters);
};

#endif // SETTINGHELPER_H
