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
    const QRegularExpression regexSetting = QRegularExpression(
        R"(^([\w\.\-\:]+)=(.*)$)",
        QRegularExpression::CaseInsensitiveOption);
    ;
    static inline SettingHelper *instance = nullptr;
    SettingHelper() {};

public:
    static SettingHelper *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new SettingHelper();
        }
        return instance;
    }
    QList<Setting> mListSettings;
    void loadSettings(const QString deviceId);
    Setting convertToSetting(const QString group, const QString line);
    void setSettings(const QList<Setting> settings, const QString deviceId);
    void setSetting(const Setting setting, const QString deviceId);
    void filterSettings(QList<Setting> &settings, const QStringList nameFilters);
};

#endif // SETTINGHELPER_H
