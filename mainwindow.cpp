#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include "Log.hpp"
#include <QTableWidgetItem>
#include <QSize>

using std::vector;

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

    bool isAllVisible = pid.isEmpty() &&
                        tag.isEmpty() &&
                        msg.isEmpty() &&
                        level.isEmpty();
    vector<Log> logs;
    if (!isAllVisible)
    {
        logs = mDataHandler.onFilterKeyChanged(pid, tag, msg, level);
    }
    mUiHandler.updateLogVisibility(ui, logs, isAllVisible);
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
    mUiHandler.clearTextInput(ui->pid, ui->tag, ui->msg, ui->level);
    ui->table_logmark->setRowCount(0);

    QString filePath = ui->file->text().trimmed();
    vector<Log> listLogs = mDataHandler.refreshLog(filePath);
    mUiHandler.loadLogs(ui, listLogs);
}

void MainWindow::onStart()
{
    QString filePath = ui->file->text().trimmed();
    bool isWatching = mDataHandler.startWatchLog(filePath);
    QString text = isWatching ? "Stop" : "Start";
    ui->start->setText(text);
    ui->clear->setDisabled(isWatching);
    mUiHandler.setDisableTextInput(isWatching /*disable*/, ui->file, ui->pid, ui->tag, ui->msg, ui->level);
    if (!isWatching)
    {
        onRefreshLog();
    }
}

void MainWindow::onClear()
{
    mDataHandler.clearLogs();
    mUiHandler.clearLogs(ui);
}

void MainWindow::onSettings()
{
    SettingDialog settings(this, ui);
    settings.exec();
}

void MainWindow::onClearMark()
{
    mUiHandler.getLineMarks(ui, mDataHandler.getFileLogHelper());
}

void MainWindow::onDownPressed(QObject *obj)
{
    QString key = mDataHandler.nextKey(ui, obj);
    if (key != "")
    {
        mUiHandler.setLineEdit(ui, obj, key);
    }
}

void MainWindow::onUpPressed(QObject *obj)
{
    QString key = mDataHandler.previousKey(ui, obj);
    if (key != "")
    {
        mUiHandler.setLineEdit(ui, obj, key);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    auto QLineEditClass = qobject_cast<QLineEdit*>(obj);
    if (QLineEditClass && event->type() == QEvent::KeyPress) {
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
    return QWidget::eventFilter(obj, event);
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

    connect(ui->table_logs, &QTableWidget::itemClicked, this, &MainWindow::onShowItem);
    connect(ui->table_logs, &QTableWidget::itemDoubleClicked, this, &MainWindow::onMarkItem);

    connect(ui->table_logmark, &QTableWidget::itemClicked, this, &MainWindow::onFocusItem);

    connect(ui->tag, &QLineEdit::returnPressed, this, &MainWindow::onSetTagHighLight);
    connect(ui->msg, &QLineEdit::returnPressed, this, &MainWindow::onSetMsgHighLight);

    connect(ui->start, &QPushButton::pressed, this, &MainWindow::onStart);
    connect(ui->clear, &QPushButton::pressed, this, &MainWindow::onClear);
    connect(ui->setting, &QPushButton::pressed, this, &MainWindow::onSettings);
    connect(ui->btn_clear_mark, &QPushButton::pressed, this, &MainWindow::onClearMark);
}
