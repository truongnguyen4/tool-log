#include "SettingHelper.hpp"
#include "ProcessHelper.hpp"
#include "Constant.hpp"
#include "Logger.hpp"
#include "NotificationHelper.hpp"

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
    int exitCode = 0;
    QString output = ProcessHelper::runShellCommand("adb", command, exitCode);
    if (exitCode != 0)
    {
        NotificationHelper::showExitCode();
        return;
    }
}

void SettingHelper::loadSettings(const QString deviceId)
{
    Logger::d(TAG, "loadSettings: deviceId = " + deviceId);
    SettingHelper::getInstance()->mListSettings.clear();
    Setting::static_line = 0;

    const QStringList groups = {
        Constant::SettingGroup::Global,
        Constant::SettingGroup::System,
        Constant::SettingGroup::Secure};

    for (const QString &group : groups)
    {
        QStringList command = {"-s", deviceId, "shell", "settings", "list", group};
        int exitCode = 0;
        QString output = ProcessHelper::runShellCommand("adb", command, exitCode);

        if (exitCode != 0)
        {
            Logger::e(TAG, QString("Failed to load settings for group '%1', exitCode: %2").arg(group).arg(exitCode));
            NotificationHelper::showExitCode();
            continue; // Try next group instead of returning immediately
        }

        const QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        for (const QString &line : lines)
        {
            Setting setting = SettingHelper::convertToSetting(group, line.trimmed());
            if (!setting.getName().isEmpty())
            { // Only add valid settings
                SettingHelper::getInstance()->mListSettings.append(setting);
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

void SettingHelper::filterSettings(QList<Setting> &settings, const QStringList names)
{
    Logger::d(TAG, "filterSetting: names = " + names.join(", "));

    for (Setting &setting : settings)
    {
        if (names.empty())
        {
            setting.setIsHidden(false);
        }
        else
        {
            bool isHidden = true;
            for (const QString &name : names)
            {
                if (setting.getName().contains(name, Qt::CaseInsensitive))
                {
                    isHidden = false;
                    break;
                }
            }
            setting.setIsHidden(isHidden);
        }
    }
}
