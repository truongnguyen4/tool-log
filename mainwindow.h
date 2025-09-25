#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "UiHandler.hpp"
#include "DataHandler.hpp"
#include <QMainWindow>
#include "NotificationDialog.hpp"
#include "SettingDialog.hpp"

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static constexpr int SUCCESS = 0;
    static constexpr int ERROR_FILE_PATH = -1;
    static constexpr int ERROR_DEVICE_ID = -2;
    static constexpr int ERROR_UNKNOWN = -3;

private:
    static const QString TAG;
    Ui::MainWindow *ui;
    UiHandler mUiHandler;
    DataHandler mDataHandler;
    SettingDialog mSettingDialog;
    NotificationDialog mNotificationDialog;

    int isWatching = 0;

    void init();
    void onRefreshDeviceIds();

    void onReloadTable();
    void onShowItem(QTableWidgetItem *item);
    void onMarkItem(QTableWidgetItem *item);
    void onFocusItem(QTableWidgetItem *item);
    void onRefreshLog();
    void onSetTagHighLight();
    void onSetMsgHighLight();
    void onStart();
    void onClear();
    void onSettings();
    void onClearMark();
    void onDownPressed(QObject *obj);
    void onUpPressed(QObject *obj);
    void onDeviceIdChanged();
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void downPressed();
    void upPressed();
};
#endif // MAINWINDOW_H
