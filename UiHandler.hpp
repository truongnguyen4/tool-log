#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP
#include "Log.hpp"
#include "./ui_mainwindow.h"
#include "FileLogHelper.hpp"
#include <vector>
#include "HighlightDelegate.hpp"

using std::vector;

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
    HighlightDelegate *mTagHLDelegate = nullptr;
    HighlightDelegate *mMsgHLDelegate = nullptr;
    void setHighLightMarkRow(QTableWidget *table, int row, QBrush foregroundColor = QBrush(), QBrush backgroundColor = QBrush());
    void initHLDelegate(Ui::MainWindow *ui);

public:
    void setTagHighLight(Ui::MainWindow *ui, const QString &tag);
    void setMsgHighLight(Ui::MainWindow *ui, const QString &msg);
    void loadLogs(Ui::MainWindow *ui, QList<Log> logs);
    void updateLogShow(Ui::MainWindow *ui, QTableWidgetItem *item);
    void markLog(Ui::MainWindow *ui, FileLogHelper &fileLogHelper, QTableWidgetItem *item = nullptr);
    void focusLog(Ui::MainWindow *ui, QTableWidgetItem *item);
    void initUi(Ui::MainWindow *ui);
    void updateLogVisibility(Ui::MainWindow *ui, QList<Log> &logs);
    void clearLogcat(Ui::MainWindow *ui);
    void setLineEdit(Ui::MainWindow *ui, QObject *obj, const QString &key);
    void clearMarkLogs(Ui::MainWindow *ui, FileLogHelper &fileLogHelper);
    void refreshDeviceIds(Ui::MainWindow *ui, QStringList deviceIds);
    void startWatching(Ui::MainWindow *ui, const bool startWatch);
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
