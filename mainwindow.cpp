#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include "Log.hpp"
#include <QTableWidgetItem>
#include <QSize>
#include "NotificationManager.hpp"

using std::vector;

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

void MainWindow::onReloadTable()
{
    QString pid = ui->pid->text().trimmed();
    QString tag = ui->tag->text().trimmed();
    QString msg = ui->msg->text().trimmed();
    QString level = ui->level->text().trimmed();

    QList<Log> logs;
    if (!pid.isEmpty() || !tag.isEmpty() || !msg.isEmpty() || !level.isEmpty())
    {
        logs = mDataHandler.onFilterKeyChanged(pid, tag, msg, level);
    }
    mUiHandler.updateLogVisibility(ui, logs);
}

void MainWindow::onShowItem(QTableWidgetItem *item)
{
    mUiHandler.updateLogShow(ui, item);
}

void MainWindow::onMarkItem(QTableWidgetItem *item)
{
    mUiHandler.markLog(ui, mDataHandler.getFileLogHelper(), item);
}

void MainWindow::onFocusItem(QTableWidgetItem *item)
{
    mUiHandler.focusLog(ui, item);
}

void MainWindow::onSetTagHighLight()
{
    QString tag = ui->tag->text().trimmed();
    mUiHandler.setTagHighLight(ui, tag);
}

void MainWindow::onSetMsgHighLight()
{
    QString msg = ui->msg->text().trimmed();
    mUiHandler.setMsgHighLight(ui, msg);
}

void MainWindow::onRefreshLog()
{
    // UI logic
    mUiHandler.clearTextInput(ui->pid, ui->tag, ui->msg, ui->level);
    ui->table_logmark->setRowCount(0);

    // Data logic
    QString filePath = ui->file->text().trimmed();
    QList<Log> listLogs = mDataHandler.refreshLog(filePath);
    mUiHandler.loadLogs(ui, listLogs);
}

void MainWindow::onStart()
{
    // Data logic
    const QString filePath = ui->file->text().trimmed();
    const QString deviceId = ui->device_ids->currentText().trimmed();
    const int errorCode = mDataHandler.startWatchLog(filePath, deviceId);
    NotificationManager::showError(errorCode);
    if (errorCode != MainWindow::SUCCESS)
    {
        return;
    } 
    isWatching = !isWatching;
    Logger::d(TAG, (isWatching ? QString("Start") : QString("Stop")) + " watching successfull");

    // UI logic
    mUiHandler.startWatching(ui, isWatching);
    if (!isWatching)
    {
        onRefreshLog();
    }
}

void MainWindow::onClear()
{
    mUiHandler.clearLogcat(ui);
    const QString deviceId = ui->device_ids->currentText().trimmed();
    int errorCode = mDataHandler.clearLogcat(deviceId);
    NotificationManager::showError(errorCode);
}

void MainWindow::onSettings()
{
    SettingDialog settings(this, ui);
    settings.exec();
}

void MainWindow::onClearMark()
{
    mUiHandler.clearMarkLogs(ui, mDataHandler.getFileLogHelper());
}

void MainWindow::onDownPressed(QObject *obj)
{
    QString key = mDataHandler.nextKey(ui, obj);
    if (!key.isEmpty())
    {
        mUiHandler.setLineEdit(ui, obj, key);
    }
}

void MainWindow::onUpPressed(QObject *obj)
{
    QString key = mDataHandler.previousKey(ui, obj);
    if (!key.isEmpty())
    {
        mUiHandler.setLineEdit(ui, obj, key);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    auto QLineEditClass = qobject_cast<QLineEdit*>(obj);
    if (QLineEditClass && event->type() == QEvent::KeyPress) {
        QString objName = obj->objectName();
        if (objName == ui->msg->objectName()
            || objName == ui->tag->objectName()
            || objName == ui->pid->objectName()
            || objName == ui->level->objectName())
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

    auto QLabelClass = qobject_cast<QLabel*>(obj);
    if (QLabelClass && event->type() == QEvent::MouseButtonPress) {
        QString objName = obj->objectName();
        if (objName == ui->devices->objectName())
        {
            onRefreshDeviceIds();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::onRefreshDeviceIds()
{
    const QStringList deviceIds = mDataHandler.getDeviceIds();
    mUiHandler.refreshDeviceIds(ui, deviceIds);
}

void MainWindow::onDeviceIdChanged()
{
    const QString deviceId = ui->device_ids->currentText();
    if (!deviceId.isEmpty())
    {
        const int errorCode = mDataHandler.deviceIdChanged(deviceId);
        NotificationManager::showError(errorCode);
    }
}

void MainWindow::init()
{
    mUiHandler.initUi(ui);

    connect(ui->file, &QLineEdit::returnPressed, this, &MainWindow::onRefreshLog);
    connect(ui->pid, &QLineEdit::returnPressed, this, &MainWindow::onReloadTable);
    connect(ui->tag, &QLineEdit::returnPressed, this, &MainWindow::onReloadTable);
    connect(ui->msg, &QLineEdit::returnPressed, this, &MainWindow::onReloadTable);
    connect(ui->level, &QLineEdit::returnPressed, this, &MainWindow::onReloadTable);

    ui->pid->installEventFilter(this);
    ui->tag->installEventFilter(this);
    ui->msg->installEventFilter(this);
    ui->level->installEventFilter(this);
    ui->devices->installEventFilter(this);

    connect(ui->table_logs, &QTableWidget::itemClicked, this, &MainWindow::onShowItem);
    connect(ui->table_logs, &QTableWidget::itemDoubleClicked, this, &MainWindow::onMarkItem);

    connect(ui->table_logmark, &QTableWidget::itemClicked, this, &MainWindow::onFocusItem);

    connect(ui->tag, &QLineEdit::returnPressed, this, &MainWindow::onSetTagHighLight);
    connect(ui->msg, &QLineEdit::returnPressed, this, &MainWindow::onSetMsgHighLight);

    connect(ui->start, &QPushButton::pressed, this, &MainWindow::onStart);
    connect(ui->clear, &QPushButton::pressed, this, &MainWindow::onClear);
    connect(ui->setting, &QPushButton::pressed, this, &MainWindow::onSettings);
    connect(ui->btn_clear_mark, &QPushButton::pressed, this, &MainWindow::onClearMark);

    connect(ui->device_ids, &QComboBox::currentTextChanged, this, &MainWindow::onDeviceIdChanged);

    onRefreshDeviceIds();
}
