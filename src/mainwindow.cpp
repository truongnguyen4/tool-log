#include <QDebug>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Logger.hpp"

QString const MainWindow::TAG = "MainWindow";

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

void MainWindow::init()
{
    mUiHandler->initUi(ui);
    ProcessHelper::getInstance()->registerDeviceChangeListener(mDeviceListener);

    connect(ui->file, &QLineEdit::returnPressed, this, &MainWindow::onRefreshLog);
    connect(ui->pid, &QLineEdit::returnPressed, this, &MainWindow::onFilterLog);
    connect(ui->tag, &QLineEdit::returnPressed, this, &MainWindow::onFilterLog);
    connect(ui->msg, &QLineEdit::returnPressed, this, &MainWindow::onFilterLog);
    connect(ui->level, &QLineEdit::returnPressed, this, &MainWindow::onFilterLog);

    connect(ui->property_filter, &QLineEdit::returnPressed, this, &MainWindow::onFilterProperties);
    connect(ui->setting_filter, &QLineEdit::returnPressed, this, &MainWindow::onFilterSettings);

    connect(ui->refresh, &QPushButton::pressed, this, [this]() {
        onRefreshProperty();
        onRefreshSetting();
    });

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
    connect(ui->save_log, &QPushButton::pressed, this, &MainWindow::onSaveLog);
    connect(ui->start_kernel, &QPushButton::pressed, this, &MainWindow::onStartKernel);
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::onStop);
}

void MainWindow::onStart()
{
    // Data logic
    const QString filePath = ui->file->text().trimmed();
    const QString deviceId = ui->device_ids->currentText().trimmed();
    if (deviceId.isEmpty() || filePath.isEmpty())
    {
        return;
    }
    isWatching = !isWatching;
    Logger::d(TAG, (QString(isWatching ? "Start" : "Stop")) + " watching successfull");
    mUiHandler->startWatching(isWatching);
    mDataHandler->startWatchLogRealTime(deviceId, isWatching);
}

void MainWindow::onStop()
{
    Logger::d(TAG, "Closing Application...");
    ProcessHelper::getInstance()->stop();
}

void MainWindow::onClear()
{
    mUiHandler->clearLogcat();
    const QString deviceId = ui->device_ids->currentText().trimmed();
    if (deviceId.isEmpty())
    {
        return;
    }
    mDataHandler->clearLogcat(deviceId);
}

void MainWindow::onSaveLog()
{
    const QString filePath = ui->file->text().trimmed();
    if (filePath.isEmpty())
    {
        return;
    }
    mDataHandler->saveLog(filePath);
}

void MainWindow::onStartKernel()
{
    const QString filePath = ui->file->text().trimmed();
    const QString deviceId = ui->device_ids->currentText().trimmed();
    if (deviceId.isEmpty() || filePath.isEmpty())
    {
        return;
    }
    isWatching = !isWatching;
    Logger::d(TAG, (QString(isWatching ? "Start" : "Stop")) + " watching successfull");
    mUiHandler->startWatchingKernel(isWatching);
}

void MainWindow::onFilterLog()
{
    QString pid = ui->pid->text().trimmed();
    QString tag = ui->tag->text().trimmed();
    QString msg = ui->msg->text().trimmed();
    QString level = ui->level->text().trimmed();
    mDataHandler->filterLogs(pid, tag, msg, level);
    mUiHandler->updateLogVisibility();
}

void MainWindow::onFilterProperties()
{
    QString name = ui->property_filter->text().trimmed();
    mDataHandler->filterProperties(name);
    mUiHandler->updatePropertiesVisibility();
}

void MainWindow::onFilterSettings()
{
    QString name = ui->setting_filter->text().trimmed();
    mDataHandler->filterSettings(name);
    mUiHandler->updateSettingsVisibility();
}

void MainWindow::onRefreshLog()
{
    // Data logic
    QString filePath = ui->file->text().trimmed();
    mDataHandler->loadLogs(filePath);
    mUiHandler->loadLogs();
    onSetTagHighLight();
    onSetMsgHighLight();
    onFind();
}

void MainWindow::onRefreshSetting()
{
    const QString deviceId = ui->device_ids->currentText().trimmed();
    if (deviceId.isEmpty())
    {
        return;
    }
    mDataHandler->loadSettings(deviceId);

    // Load all Settings in the fist time or only update them
    if (!mUiHandler->isFirstLoadSettings)
    {
        mUiHandler->loadSettings();
        mUiHandler->isFirstLoadSettings = true;
    }
    else
    {
        mUiHandler->updateValueSettings();
    }
}

void MainWindow::onRefreshProperty()
{
    const QString deviceId = ui->device_ids->currentText().trimmed();
    if (deviceId.isEmpty())
    {
        return;
    }
    mDataHandler->loadProperties(deviceId);
    // Load all Properties in the fist time or only update them
    if (!mUiHandler->isFirstLoadProperties)
    {
        mUiHandler->loadProperties();
        mUiHandler->isFirstLoadProperties = true;
    }
    else
    {
        mUiHandler->updateValueProperties();
    }
}

void MainWindow::onFind()
{
    QString find = ui->find->text();
    mUiHandler->highlightFind(find);
}

void MainWindow::onSetTagHighLight()
{
    QString tag = ui->tag->text().trimmed();
    mUiHandler->highlightTag(tag);
}

void MainWindow::onSetMsgHighLight()
{
    QString msg = ui->msg->text().trimmed();
    mUiHandler->highlightMsg(msg);
}

void MainWindow::onMarkItem(QTableWidgetItem *item)
{
    mUiHandler->toggleMarkLog(item);
}

void MainWindow::onFocusItem(QTableWidgetItem *item)
{
    mUiHandler->focusLog(item);
}

void MainWindow::onClearMark()
{
    mUiHandler->clearMarkLogs();
}

void MainWindow::onDownPressed(QObject *obj)
{
    QString key = mDataHandler->nextKey(ui, obj);
    if (!key.isEmpty())
    {
        static_cast<QLineEdit *>(obj)->setText(key);
    }
}

void MainWindow::onUpPressed(QObject *obj)
{
    QString key = mDataHandler->previousKey(ui, obj);
    if (!key.isEmpty())
    {
        static_cast<QLineEdit *>(obj)->setText(key);
    }
}
void MainWindow::onSettings()
{
    SettingDialog settings(this, ui);
    settings.exec();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    auto QLineEditClass = qobject_cast<QLineEdit *>(obj);
    if (QLineEditClass && event->type() == QEvent::KeyPress)
    {
        QString objName = obj->objectName();
        if (objName == ui->find->objectName() 
            || objName == ui->msg->objectName() 
            || objName == ui->tag->objectName() 
            || objName == ui->pid->objectName() 
            || objName == ui->level->objectName() 
            || objName == ui->property_filter->objectName() 
            || objName == ui->setting_filter->objectName())
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Down)
            {
                onDownPressed(obj);
                return true;
            }
            if (keyEvent->key() == Qt::Key_Up)
            {
                onUpPressed(obj);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::onChangeConnectDevices(QStringList deviceIds, const bool isConnected)
{
    mUiHandler->refreshDeviceIds(deviceIds, isConnected);
}

void MainWindow::onShowItem(QTableWidgetItem *item)
{
    mUiHandler->updateLogShow(item);
}
