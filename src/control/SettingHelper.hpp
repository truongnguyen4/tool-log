#ifndef SETTINGHELPER_H
#define SETTINGHELPER_H
#include "Setting.hpp"
#include <QList>
#include <QString>
#include <QRegularExpression>
#include "UtilHelper.hpp"
#include "ProcessHelper.hpp"

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
    UtilHelper *mUtilHelper = UtilHelper::getInstance();
    ProcessHelper *mProcessHelper = ProcessHelper::getInstance();

public:
    static SettingHelper *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new SettingHelper();
        }
        return instance;
    }
    QList<Setting> mListObjs;
    QStringList mListKeys;
    bool andOp = false;
    void clearSettings()
    {
        Setting::static_id = 0;
        mListObjs.clear();
    }
    void loadSettings(const QString deviceId);
    Setting convertToSetting(const QString group, const QString line);
    void setSettings(const QList<Setting> settings, const QString deviceId);
    void setSetting(const Setting setting, const QString deviceId);
    void filterSettings();
};

#endif // SETTINGHELPER_H
