#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP
#include "Log.hpp"
#include "ui_mainwindow.h"
#include "HighlightKey.hpp"
#include "HighlightCell.hpp"
#include "Setting.hpp"
#include "Property.hpp"
#include "UtilHelper.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class UiHandler;
}
QT_END_NAMESPACE
class UiHandler
{
private:
    static const inline QString TAG = "UiHandler";
    static inline UiHandler *instance = nullptr;
    int LINE_HEIGTH = 30;
    QColor MARK_LOG_FOREGROUND_COLOR = QColorConstants::Black;
    QColor MARK_LOG_BACKGROUND_COLOR = QColorConstants::LightGray;
    HighlightKey *mTagHLDelegate = nullptr;
    HighlightKey *mMsgHLDelegate = nullptr;
    HighlightCell *mLevelHLDelegate = nullptr;
    UtilHelper *mUtilHelper = UtilHelper::getInstance();
    Ui::MainWindow *mUi = nullptr;
    void setHighLightMarkRow(QTableWidget *table, int row, QBrush foregroundColor = QBrush(), QBrush backgroundColor = QBrush());
    void initHLDelegate();
    UiHandler() {};

public:
    static UiHandler *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new UiHandler();
        }
        return instance;
    }
    bool isFirstLoadSettings = false;
    bool isFirstLoadProperties = false;

    void highlightFind(const QString key);
    void highlightTag(const QString &tag);
    void highlightMsg(const QString &msg);

    void loadLogs();
    void updateLogVisibility();
    void insertLogToTable(const Log &log, const int row);
    void insertLogToTable(const QList<Log> logs);

    void loadSettings();
    void updateValueSettings();
    void insertSettingToTable(const Setting setting, const int row);
    void updateSettingsVisibility();

    void loadProperties();
    void updateValueProperties();
    void insertPropertyToTable(const Property property, const int row);
    void updatePropertiesVisibility();

    void updateLogShow(QTableWidgetItem *item);
    void toggleMarkLog(QTableWidgetItem *item = nullptr);
    void markLog(const Log log, const int row);
    void unmarkLog(const Log log);
    void focusLog(QTableWidgetItem *item);
    void initUi(Ui::MainWindow *ui);
    void clearLogcat();
    void clearMarkLogs();
    void refreshDeviceIds(QStringList deviceIds, const bool isConnected);
    void startWatching(const bool startWatch);
    void startWatchingKernel(const bool startWatch);
};

#endif // UIHANDLER_HPP
