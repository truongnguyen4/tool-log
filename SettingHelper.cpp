#include "SettingHelper.hpp"
#include "ProcessHelper.hpp"
#include "Constant.hpp"
#include "Logger.hpp"
#include "NotificationHelper.hpp"
#include "UtilHelper.hpp"

void SettingHelper::setSettings(const QList<Setting> settings, const QString deviceId)
{
    for (const Setting &setting : settings)
    {
        setSetting(setting, deviceId);
    }
}

void SettingHelper::setSetting(const Setting setting, const QString deviceId)
{
    QStringList command = {"-s", deviceId, "shell", "settings", "put", setting.getGroup(), setting.getName(), setting.getValue()};
    QString output = mProcessHelper->runShellCommand("adb", command);
}

void SettingHelper::loadSettings(const QString deviceId)
{
    Logger::d(TAG, "loadSettings: deviceId = " + deviceId);

    const QStringList groups = {
        Constant::SettingGroup::Global,
        Constant::SettingGroup::System,
        Constant::SettingGroup::Secure};

    for (const QString &group : groups)
    {
        QStringList command = {"-s", deviceId, "shell", "settings", "list", group};
        QString output = mProcessHelper->runShellCommand("adb", command);
        const QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        for (const QString &line : lines)
        {
            Setting setting = SettingHelper::convertToSetting(group, line.trimmed());
            if (!setting.getName().isEmpty())
            {
                SettingHelper::getInstance()->mListObjs.append(setting);
            }
        }
    }
}

Setting SettingHelper::convertToSetting(const QString group, const QString line)
{
    QRegularExpressionMatch match = regexSetting.match(line);
    if (match.hasMatch())
    {
        return Setting(group, match.captured(1), match.captured(2));
    }
    Logger::w(TAG, "Can't convert setting, line: " + line);
    return Setting();
}

void SettingHelper::filterSettings()
{
    Logger::d(TAG, "filterSetting: mSettings = " + mListKeys.join(", "));
    for (Setting &setting : mListObjs)
    {
        setting.setHidden(false);
        mUtilHelper->updateHidden<Setting>(setting, setting.getName(), mListKeys, andOp);
    }
}
