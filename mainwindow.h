#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "UiHandler.hpp"
#include "DataHandler.hpp"
#include <QMainWindow>
#include "NotificationDialog.hpp"
#include "SettingDialog.hpp"
#include <QAtomicInteger>
#include "ProcessHelper.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    static const QString TAG;
    Ui::MainWindow *ui;
    UiHandler *mUiHandler = UiHandler::getInstance();
    DataHandler *mDataHandler = DataHandler::getInstance();
    class MainWindowDeviceChangeListener : public ProcessHelper::DeviceChangeListener
    {
    public:
        MainWindowDeviceChangeListener(MainWindow *mw) : mainWindow(mw) {}
        void onDevicesIsConnected(QStringList deviceIds) override
        {
            if (mainWindow)
            {
                mainWindow->onChangeConnectDevices(deviceIds, true);
            }
        }
        void onDevicesIsDisconnected(QStringList deviceIds) override
        {
            if (mainWindow)
            {
                mainWindow->onChangeConnectDevices(deviceIds, false);
            }
        }
    private:
        MainWindow *mainWindow;
    };

    MainWindowDeviceChangeListener *mDeviceListener = new MainWindowDeviceChangeListener(this);

    bool isWatching = false;

    void init();
    void onStart();
    void onStop();
    void onClear();
    void onSaveLog();
    void onStartKernel();
    
    void onFilterLog();
    void onFilterProperties();
    void onFilterSettings();

    void onRefreshLog();
    void onRefreshSetting();
    void onRefreshProperty();
    
    void onFind();
    void onSetTagHighLight();
    void onSetMsgHighLight();
    
    void onMarkItem(QTableWidgetItem *item);
    void onFocusItem(QTableWidgetItem *item);
    void onClearMark();

    void onChangeConnectDevices(QStringList deviceIds, const bool isConnected);
    void onShowItem(QTableWidgetItem *item);
    void onSettings();
    void onDownPressed(QObject *obj);
    void onUpPressed(QObject *obj);
    bool eventFilter(QObject *obj, QEvent *event);
};
#endif // MAINWINDOW_H
