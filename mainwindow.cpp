#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include "Log.hpp"
#include <QTableWidgetItem>
#include "NotificationHelper.hpp"
#include <QDateTime>
#include "Logger.hpp"
#include "Property.hpp"
#include "Setting.hpp"
#include <QDateTime>

QString const MainWindow::TAG = "MainWindow";
QStringList MainWindow::simulateDevices = {};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFilterLog()
{
    QString pid = ui->pid->text().trimmed();
    QString tag = ui->tag->text().trimmed();
    QString msg = ui->msg->text().trimmed();
    QString level = ui->level->text().trimmed();
    mDataHandler.addKey(pid, tag, msg, level);
    QList<Log> logs = mDataHandler.filterLogs(pid, tag, msg, level);
    mUiHandler.updateLogVisibility(logs);
}

void MainWindow::onShowItem(QTableWidgetItem *item)
{
    mUiHandler.updateLogShow(item);
}

void MainWindow::onMarkItem(QTableWidgetItem *item)
{
    mUiHandler.markLog(item);
}

void MainWindow::onFocusItem(QTableWidgetItem *item)
{
    mUiHandler.focusLog(item);
}

void MainWindow::onSetTagHighLight()
{
    QString tag = ui->tag->text().trimmed();
    mUiHandler.setTagHighLight(tag);
}

void MainWindow::onSetMsgHighLight()
{
    QString msg = ui->msg->text().trimmed();
    mUiHandler.setMsgHighLight(msg);
}

void MainWindow::onRefreshLog()
{
    // UI logic
    ui->table_logmark->setRowCount(0);

    // Data logic
    QString filePath = ui->file->text().trimmed();
    QList<Log> listLogs = mDataHandler.refreshLog(filePath);
    mUiHandler.loadLogs(listLogs);
    onFilterLog();
    onSetTagHighLight();
    onSetMsgHighLight();
    onFind();
}

void MainWindow::onStart()
{
    // Data logic
    const QString filePath = ui->file->text().trimmed();
    const QString deviceId = ui->device_ids->currentText().trimmed();
    if (deviceId.isEmpty())
    {
        return;
    }
    const int errorCode = mDataHandler.startWatchLog(filePath, deviceId);
    NotificationHelper::showError(errorCode);
    if (errorCode != MainWindow::SUCCESS)
    {
        return;
    }
    isWatching = !isWatching;
    Logger::d(TAG, (isWatching ? QString("Start") : QString("Stop")) + " watching successfull");

    // UI logic
    mUiHandler.startWatching(isWatching);
    if (!isWatching)
    {
        onRefreshLog();
    }
}

void MainWindow::onClear()
{
    mUiHandler.clearLogcat();
    const QString deviceId = ui->device_ids->currentText().trimmed();
    if (deviceId.isEmpty())
    {
        return;
    }
    int errorCode = mDataHandler.clearLogcat(deviceId);
    NotificationHelper::showError(errorCode);
}

void MainWindow::onSettings()
{
    SettingDialog settings(this, ui);
    settings.exec();
}

void MainWindow::onClearMark()
{
    mUiHandler.clearMarkLogs();
}

void MainWindow::onDownPressed(QObject *obj)
{
    QString key = mDataHandler.nextKey(ui, obj);
    if (!key.isEmpty())
    {
        static_cast<QLineEdit*>(obj)->setText(key);
    }
}

void MainWindow::onUpPressed(QObject *obj)
{
    QString key = mDataHandler.previousKey(ui, obj);
    if (!key.isEmpty())
    {
        static_cast<QLineEdit*>(obj)->setText(key);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    auto QLineEditClass = qobject_cast<QLineEdit*>(obj);
    if (QLineEditClass && event->type() == QEvent::KeyPress) {
        QString objName = obj->objectName();
        if (objName == ui->find->objectName()
            || objName == ui->msg->objectName()
            || objName == ui->tag->objectName()
            || objName == ui->pid->objectName()
            || objName == ui->level->objectName()
            || objName == ui->property_filter->objectName()
            || objName == ui->setting_filter->objectName())
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Down) {
                onDownPressed(obj);
                return true;
            }
            if (keyEvent->key() == Qt::Key_Up) {
                onUpPressed(obj);
                return true;
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void MainWindow::onChangeConnectDevices(QStringList deviceIds, const bool isConnected)
{
    mUiHandler.refreshDeviceIds(deviceIds, isConnected);
}

void MainWindow::onStop()
{
    Logger::d(TAG, "Closing Application...");
    mProcessHandler.stop();
}

void MainWindow::onFind()
{
    QString find = ui->find->text();
    mDataHandler.addKey(find);
    mUiHandler.setFindHighlight(find);
}

void MainWindow::onRefreshSettingProperty()
{
    const QString deviceId = ui->device_ids->currentText().trimmed();
    if (deviceId.isEmpty())
    {
        return;
    }
    QList<Property> properties = mDataHandler.loadProperties(deviceId);
    QList<Setting> settings = mDataHandler.loadSettings(deviceId);
    if (!mUiHandler.isFirstLoadSettings) {
        mUiHandler.loadSettings(settings);
        mUiHandler.isFirstLoadSettings = true;
    } else {
        mUiHandler.updateValueSettings(settings);
    }
    if (!mUiHandler.isFirstLoadProperties) {
        mUiHandler.loadProperties(properties);
        mUiHandler.isFirstLoadProperties = true;
    } else {
        mUiHandler.updateValueProperties(properties);
    }
    onFilterSettings();
    onFilterProperties();
}

void MainWindow::onFilterSettings()
{
    QString name = ui->setting_filter->text().trimmed();
    mDataHandler.addKey("", name);
    QList<Setting> settings = mDataHandler.filterSettings(name);
    mUiHandler.updateSettingsVisibility(settings);
}

void MainWindow::onFilterProperties()
{
    QString name = ui->property_filter->text().trimmed();
    mDataHandler.addKey(name, "");
    QList<Property> properties = mDataHandler.filterProperties(name);
    mUiHandler.updatePropertiesVisibility(properties);
}

void MainWindow::init()
{
    mUiHandler.initUi(ui);
    mProcessHandler.registerDeviceChangeListener(mDeviceListener);

    connect(ui->file, &QLineEdit::returnPressed, this, &MainWindow::onRefreshLog);
    connect(ui->pid, &QLineEdit::returnPressed, this, &MainWindow::onFilterLog);
    connect(ui->tag, &QLineEdit::returnPressed, this, &MainWindow::onFilterLog);
    connect(ui->msg, &QLineEdit::returnPressed, this, &MainWindow::onFilterLog);
    connect(ui->level, &QLineEdit::returnPressed, this, &MainWindow::onFilterLog);

    connect(ui->property_filter, &QLineEdit::returnPressed, this, &MainWindow::onFilterProperties);
    connect(ui->setting_filter, &QLineEdit::returnPressed, this, &MainWindow::onFilterSettings);

    connect(ui->refresh, &QPushButton::pressed, this, &MainWindow::onRefreshSettingProperty);

    ui->find->installEventFilter(this);
    ui->pid->installEventFilter(this);
    ui->tag->installEventFilter(this);
    ui->msg->installEventFilter(this);
    ui->level->installEventFilter(this);
    ui->devices->installEventFilter(this);
    ui->property_filter->installEventFilter(this);
    ui->setting_filter->installEventFilter(this);

    connect(ui->table_logs, &QTableWidget::itemClicked, this, &MainWindow::onShowItem);
    connect(ui->table_logs, &QTableWidget::itemDoubleClicked, this, &MainWindow::onMarkItem);
    connect(ui->table_logmark, &QTableWidget::itemClicked, this, &MainWindow::onFocusItem);
    connect(ui->find, &QLineEdit::returnPressed, this, &MainWindow::onFind);
    connect(ui->tag, &QLineEdit::returnPressed, this, &MainWindow::onSetTagHighLight);
    connect(ui->msg, &QLineEdit::returnPressed, this, &MainWindow::onSetMsgHighLight);
    connect(ui->start, &QPushButton::pressed, this, &MainWindow::onStart);
    connect(ui->clear, &QPushButton::pressed, this, &MainWindow::onClear);
    connect(ui->setting, &QPushButton::pressed, this, &MainWindow::onSettings);
    connect(ui->btn_clear_mark, &QPushButton::pressed, this, &MainWindow::onClearMark);
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::onStop);
}
