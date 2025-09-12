#include "UiHandler.hpp"
#include <QTableWidgetItem>
#include "Logger.hpp"
#include <string>
#include <QScrollBar>
#include "Constant.hpp"

const QString UiHandler::TAG = "UiHandler";
//============== Public methods ==============

void UiHandler::setTagHighLight(Ui::MainWindow *ui, const QString &tag)
{
    Logger::d(TAG,  QString::fromStdString("setTagHighLight is called with tag = " + tag.toStdString()));
    initHLDelegate(ui);
    mTagHLDelegate->setKeywords(tag);
    ui->table_logs->viewport()->update();
}

void UiHandler::setMsgHighLight(Ui::MainWindow *ui, const QString &msg)
{
    Logger::d(TAG, QString::fromStdString("setMsgHighLight is called with msg = " + msg.toStdString()));
    initHLDelegate(ui);
    mMsgHLDelegate->setKeywords(msg);
    ui->table_logs->viewport()->update();
}

void UiHandler::loadLogs(Ui::MainWindow *ui, QList<Log> logs)
{
    ui->table_logs->setRowCount(0);
    ui->table_logs->setUpdatesEnabled(false);
    for (const Log &log : logs)
    {
        const int row = ui->table_logs->rowCount();
        ui->table_logs->insertRow(row);
        ui->table_logs->setItem(row, Constant::TableLog::COL_LINE, new QTableWidgetItem(QString::number(log.getLine())));
        ui->table_logs->setItem(row, Constant::TableLog::COL_DATE, new QTableWidgetItem(QString::fromStdString(log.getDate())));
        ui->table_logs->setItem(row, Constant::TableLog::COL_TIME, new QTableWidgetItem(QString::fromStdString(log.getTime())));
        ui->table_logs->setItem(row, Constant::TableLog::COL_PID, new QTableWidgetItem(QString::fromStdString(log.getPid())));
        ui->table_logs->setItem(row, Constant::TableLog::COL_TID, new QTableWidgetItem(QString::fromStdString(log.getTid())));
        ui->table_logs->setItem(row, Constant::TableLog::COL_LEVEL, new QTableWidgetItem(QString::fromStdString(log.getLevel())));
        ui->table_logs->setItem(row, Constant::TableLog::COL_TAG, new QTableWidgetItem(QString::fromStdString(log.getTag())));
        ui->table_logs->setItem(row, Constant::TableLog::COL_MSG, new QTableWidgetItem(QString::fromStdString(log.getMsg())));
    }
    ui->table_logs->setUpdatesEnabled(true);
}

void UiHandler::updateLogShow(Ui::MainWindow *ui, QTableWidgetItem *item)
{
    ui->log->setText(item->text());
}

void UiHandler::markLog(Ui::MainWindow *ui, FileLogHelper &fileLogHelper, QTableWidgetItem *item)
{
    const int row = item->row();
    const bool isMarked = fileLogHelper.reverseIsMarkLog(row);
    const int rows = ui->table_logmark->rowCount();
    const Log log = fileLogHelper.getLog(row);

    if (isMarked)
    {
        // mark log to table
        ui->table_logmark->insertRow(rows);
        QTableWidgetItem *itemLine = new QTableWidgetItem();
        itemLine->setData(Qt::DisplayRole, log.getLine());
        ui->table_logmark->setItem(rows, Constant::TableLogMark::COL_LINE, itemLine);
        ui->table_logmark->setItem(rows, Constant::TableLogMark::COL_PID, new QTableWidgetItem(QString::fromStdString(log.getPid())));
        ui->table_logmark->setItem(rows, Constant::TableLogMark::COL_TAG, new QTableWidgetItem(QString::fromStdString(log.getTag())));
        ui->table_logmark->setItem(rows, Constant::TableLogMark::COL_MSG, new QTableWidgetItem(QString::fromStdString(log.getMsg())));

        ui->table_logmark->setSortingEnabled(true);   // enable sorting to sort table logmark
        ui->table_logmark->sortItems(Constant::TableLogMark::COL_LINE, Qt::SortOrder::AscendingOrder);
        ui->table_logmark->setSortingEnabled(false);  // disable sorting after completing sorting

        // set foreground and background color of log in table logs
        setHighLightMarkRow(ui->table_logs, row, MARK_LOG_FOREGROUND_COLOR, MARK_LOG_BACKGROUND_COLOR);
        Logger::d(TAG,  QString::fromStdString("Mark line " + std::to_string(log.getLine())));
    }
    else
    {
        // unmark log from table log mark
        for (int row = 0; row < rows; row++)
        {
            QTableWidgetItem *itemLine = ui->table_logmark->item(row, Constant::TableLogMark::COL_LINE);
            if (itemLine && itemLine->data(Qt::DisplayRole).toInt() == log.getLine())
            {
                ui->table_logmark->removeRow(row);   // remove row from table logmark
                setHighLightMarkRow(ui->table_logs, log.getLine()-1);  // reset foreground and background color of log in table logs
                Logger::d(TAG, QString::fromStdString("Unmark line " + std::to_string(log.getLine())));
                break;
            }
        }
    }
}

void UiHandler::focusLog(Ui::MainWindow *ui, QTableWidgetItem *item)
{
    int line = ui->table_logmark->item(item->row(), 0)->data(Qt::DisplayRole).toInt();
    int targetRow = line - 1;

    // Ensure the target row is not hidden
    while (targetRow > 0 && ui->table_logs->isRowHidden(targetRow))
    {
        targetRow -= 1;
    }
    Logger::d(TAG,  QString::fromStdString("targetRow = " + std::to_string(targetRow)));

    // If the target row is hidden, scroll to the top
    if (ui->table_logs->isRowHidden(targetRow))
    {
        ui->table_logs->verticalScrollBar()->setValue(0);
        return;
    }

    // Scroll to the target row and set it as current
    QTableWidgetItem *itemFocus = ui->table_logs->item(targetRow, Constant::TableLog::COL_LINE);
    QModelIndex modelIndex = ui->table_logs->indexFromItem(itemFocus);
    ui->table_logs->scrollTo(modelIndex, QAbstractItemView::PositionAtCenter);
    ui->table_logs->setCurrentCell(targetRow, Constant::TableLog::COL_LEVEL);
}

void UiHandler::initUi(Ui::MainWindow *ui)
{
    ui->file->setText("/home/truongnguyen/Downloads/output.log");

    // Table logs
    ui->table_logs->setColumnCount(8);
    QStringList logHeaders = { Constant::TableLog::LINE,
                                Constant::TableLog::DATE,
                                Constant::TableLog::TIME,
                                Constant::TableLog::PID,
                                Constant::TableLog::TID,
                                Constant::TableLog::LEVEL,
                                Constant::TableLog::TAG,
                                Constant::TableLog::MSG };
    ui->table_logs->setHorizontalHeaderLabels(logHeaders);
    ui->table_logs->verticalHeader()->setVisible(false);
    ui->table_logs->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_logs->setWordWrap(false);
    ui->table_logs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_logs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_logs->setSortingEnabled(false);
    ui->table_logs->setMouseTracking(false);
    ui->table_logs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->table_logs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->table_logs->setAutoScroll(false);
    ui->table_logs->setColumnHidden(Constant::TableLog::COL_DATE, true);
    ui->table_logs->setColumnHidden(Constant::TableLog::COL_TIME, true);
    ui->table_logs->setColumnHidden(Constant::TableLog::COL_TID, true);
    ui->table_logs->setStyleSheet(R"(
        QTableWidget::item:selected {
            background-color: #3399ff;
            color: white;
        }
    )");

    // Table logs mark
    ui->table_logmark->setColumnCount(4);
    QStringList markHeaders = { Constant::TableLogMark::LINE,
                                Constant::TableLogMark::PID,
                                Constant::TableLogMark::TAG,
                                Constant::TableLogMark::MSG };
    ui->table_logmark->setHorizontalHeaderLabels(markHeaders);
    ui->table_logmark->verticalHeader()->setVisible(false);
    ui->table_logmark->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_logmark->setWordWrap(false);
    ui->table_logmark->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_logmark->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_logmark->setSortingEnabled(false);
    ui->table_logmark->setMouseTracking(false);
    ui->table_logmark->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->table_logmark->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->table_logmark->setAutoScroll(false);

    // Line Text
    ui->file->setFixedHeight(LINE_HEIGTH);
    ui->pid->setFixedHeight(LINE_HEIGTH);
    ui->tag->setFixedHeight(LINE_HEIGTH);
    ui->msg->setFixedHeight(LINE_HEIGTH);
    ui->level->setFixedHeight(LINE_HEIGTH);

    // Log
    ui->log->setFixedHeight(LINE_HEIGTH*2);
    ui->log->setReadOnly(true);
}

void UiHandler::updateLogVisibility(Ui::MainWindow *ui, QList<Log> &logs)
{
    if (logs.isEmpty())
    {
        ui->table_logs->setUpdatesEnabled(false);
        for (int row = 0; row < ui->table_logs->rowCount(); ++row)
        {
            ui->table_logs->setRowHidden(row, false);
        }
        ui->table_logs->setUpdatesEnabled(true);
        return;
    }

    Logger::setTimeFrom("Set hidden", steady_clock::now());
    ui->table_logs->setUpdatesEnabled(false);
    for (const Log &log : logs)
    {
        ui->table_logs->setRowHidden(log.getLine() - 1, log.getHidden());
    }
    ui->table_logs->setUpdatesEnabled(true);
    Logger::setTimeTo("Set hidden", steady_clock::now());
}

void UiHandler::clearLogs(Ui::MainWindow *ui)
{
    clearTextInput(ui->pid, ui->tag, ui->msg, ui->level);
    ui->table_logmark->clearContents();
    ui->table_logs->clearContents();
    ui->log->clear();
}

void UiHandler::setLineEdit(Ui::MainWindow *ui, QObject *obj, const QString &key)
{
    if (obj == ui->pid)
    {
        ui->pid->setText(key);
    }
    if (obj == ui->tag)
    {
        ui->tag->setText(key);
    }
    if (obj == ui->msg)
    {
        ui->msg->setText(key);
    }
    if (obj == ui->level)
    {
        ui->level->setText(key);
    }
}

void UiHandler::getLineMarks(Ui::MainWindow *ui, FileLogHelper &fileLogHelper)
{
    const int rows = ui->table_logmark->rowCount();
    for (int row = 0; row < rows; row++)
    {
        QTableWidgetItem *itemLine = ui->table_logmark->item(row, Constant::TableLogMark::COL_LINE);
        if (itemLine)
        {
            int row = itemLine->data(Qt::DisplayRole).toInt() - 1;
            setHighLightMarkRow(ui->table_logs, row);
            fileLogHelper.reverseIsMarkLog(row);
        }
    }
    ui->table_logmark->setRowCount(0);
}

//===========================================================
//============== Private methods ============================
//===========================================================
void UiHandler::setHighLightMarkRow(QTableWidget *table, int row, QBrush foregroundColor, QBrush backgroundColor)
{
    if (row < 0 || row >= table->rowCount())
    {
        return;
    }
    for (int col = 0; col < table->columnCount(); ++col)
    {
        if (QTableWidgetItem *item = table->item(row, col))
        {
            item->setForeground(foregroundColor);
            item->setBackground(backgroundColor);
        }
    }
}

void UiHandler::initHLDelegate(Ui::MainWindow *ui)
{
    if (mTagHLDelegate == nullptr)
    {
        Logger::d(TAG, "mTagHLDelegate is nullptr, creating a new one");
        mTagHLDelegate = new HighlightDelegate(ui->table_logs);
        mTagHLDelegate->setKeywords("");
        ui->table_logs->setItemDelegateForColumn(Constant::TableLog::COL_TAG, mTagHLDelegate);
    }
    if (mMsgHLDelegate == nullptr)
    {
        Logger::d(TAG, "mMsgHLDelegate is nullptr, creating a new one");
        mMsgHLDelegate = new HighlightDelegate(ui->table_logs);
        mMsgHLDelegate->setKeywords("");
        ui->table_logs->setItemDelegateForColumn(Constant::TableLog::COL_MSG, mMsgHLDelegate);
    }
}
