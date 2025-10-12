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
    void highlightFindKey(const QString key);
    void setTagHighLight(const QString &tag);
    void setMsgHighLight(const QString &msg);
    void loadLogs(QList<Log> logs);
    void updateLogShow(QTableWidgetItem *item);
    void markLog(QTableWidgetItem *item = nullptr);
    void focusLog(QTableWidgetItem *item);
    void initUi(Ui::MainWindow *ui);
    void updateLogVisibility(QList<Log> &logs);
    void clearLogcat();
    void setLineEdit(QObject *obj, const QString &key);
    void clearMarkLogs();
    void refreshDeviceIds(QStringList deviceIds, const bool isConnected);
    void startWatching(const bool startWatch);
    void refreshSettingProperty(const QList<Setting> settings, const QList<Property> properties);
    template <typename... Args>
    void clearTextInput(Args &&...textBoxes)
    {
        (textBoxes->setText(""), ...);
    }
    template <typename... Args>
    void setDisableTextInput(bool disable, Args &&...textBoxes)
    {
        (textBoxes->setDisabled(disable), ...);
    }
    template <typename... Args>
    void setDisableComboBox(bool disable, Args &&...comboBoxes)
    {
        (comboBoxes->setDisabled(disable), ...);
    }
};

#endif // UIHANDLER_HPP
