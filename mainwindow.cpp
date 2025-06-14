#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include "Log.hpp"
#include <QTableWidgetItem>
#include <QSize>

using std::vector;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

    if (pid.isEmpty() && tag.isEmpty() && msg.isEmpty() && level.isEmpty())
    {
        Logger::w("MainWindow", "No filter criteria provided. Please enter at least one filter.");
        return;
    }

    vector<Log> logs = mDataHandler.onFilterKeyChanged(pid, tag, msg, level);
    Logger::setTimeFrom("Set hidden", steady_clock::now());
    ui->table_logs->setUpdatesEnabled(false);
    for (const Log &log : logs)
    {
        ui->table_logs->setRowHidden(log.getLine() - 1, log.getHidden());
    }
    ui->table_logs->setUpdatesEnabled(true);
    Logger::setTimeTo("Set hidden", steady_clock::now());
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
    ui->pid->setText("");
    ui->tag->setText("");
    ui->msg->setText("");
    ui->level->setText("");
    ui->log->setText("");

    QString filePath = ui->file->text().trimmed();
    vector<Log> listLogs = mDataHandler.refreshLog(filePath);
    mUiHandler.loadLogs(ui, listLogs);
}

void MainWindow::init()
{
    mUiHandler.initUi(ui);

    connect(ui->file, &QLineEdit::returnPressed, this, &MainWindow::onRefreshLog);
    // connect(ui->pid, &QLineEdit::returnPressed, this, &MainWindow::onReloadTable);
    // connect(ui->tag, &QLineEdit::returnPressed, this, &MainWindow::onReloadTable);
    // connect(ui->msg, &QLineEdit::returnPressed, this, &MainWindow::onReloadTable);
    // connect(ui->level, &QLineEdit::returnPressed, this, &MainWindow::onReloadTable);

    connect(ui->table_logs, &QTableWidget::itemClicked, this, &MainWindow::onShowItem);
    connect(ui->table_logs, &QTableWidget::itemDoubleClicked, this, &MainWindow::onMarkItem);

    connect(ui->table_logmark, &QTableWidget::itemClicked, this, &MainWindow::onFocusItem);

    connect(ui->tag, &QLineEdit::returnPressed, this, &MainWindow::onSetTagHighLight);
    connect(ui->msg, &QLineEdit::returnPressed, this, &MainWindow::onSetMsgHighLight);
}


