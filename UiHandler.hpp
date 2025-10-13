#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP
#include "Log.hpp"
#include "./ui_mainwindow.h"
#include "HighlightKey.hpp"
#include "HighlightCell.hpp"
#include "Setting.hpp"
#include "Property.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class UiHandler;
}
QT_END_NAMESPACE
class UiHandler
{
private:
    static const QString TAG;
    int LINE_HEIGTH = 30;
    QColor MARK_LOG_FOREGROUND_COLOR = QColorConstants::Black;
    QColor MARK_LOG_BACKGROUND_COLOR = QColorConstants::LightGray;
    HighlightKey *mTagHLDelegate = nullptr;
    HighlightKey *mMsgHLDelegate = nullptr;
    HighlightCell *mLevelHLDelegate = nullptr;
    Ui::MainWindow *mUi = nullptr;
    void setHighLightMarkRow(QTableWidget *table, int row, QBrush foregroundColor = QBrush(), QBrush backgroundColor = QBrush());
    void initHLDelegate();

public:
    bool isFirstLoadSettings = false;
    bool isFirstLoadProperties = false;
    void setFindHighlight(const QString key);
    void setTagHighLight(const QString &tag);
    void setMsgHighLight(const QString &msg);
    void loadLogs(QList<Log> logs);
    void updateLogVisibility(QList<Log> &logs);
    void updateLogShow(QTableWidgetItem *item);
    void markLog(QTableWidgetItem *item = nullptr);
    void focusLog(QTableWidgetItem *item);
    void initUi(Ui::MainWindow *ui);
    void clearLogcat();
    void clearMarkLogs();
    void refreshDeviceIds(QStringList deviceIds, const bool isConnected);
    void startWatching(const bool startWatch);
    void updateSettingsVisibility(const QList<Setting> settings);
    void updatePropertiesVisibility(const QList<Property> properties);
    void loadSettings(const QList<Setting> settings);
    void loadProperties(const QList<Property> properties);
    void updateValueSettings(const QList<Setting> settings);
    void updateValueProperties(const QList<Property> properties);
};

#endif // UIHANDLER_HPP
