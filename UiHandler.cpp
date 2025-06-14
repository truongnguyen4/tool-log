#include "UiHandler.hpp"
#include <QTableWidgetItem>
#include "Logger.hpp"
#include <string>
#include <QScrollBar>

void UiHandler::setTagHighLight(Ui::MainWindow *ui, const QString &tag)
{
    Logger::d("UiHandler", "setTagHighLight is called with tag = " + tag.toStdString());
    if (mTagHLDelegate == nullptr || mMsgHLDelegate == nullptr)
    {
        mTagHLDelegate = new HighlightDelegate(ui->table_logs);
        mMsgHLDelegate = new HighlightDelegate(ui->table_logs);
        mTagHLDelegate->setKeywords(tag);
        ui->table_logs->setItemDelegateForColumn(6, mTagHLDelegate);
        mMsgHLDelegate->setKeywords("");
        ui->table_logs->setItemDelegateForColumn(7, mMsgHLDelegate);
    } else {
        mTagHLDelegate->setKeywords(tag);
    }
    ui->table_logs->viewport()->update();
}

void UiHandler::setMsgHighLight(Ui::MainWindow *ui, const QString &msg)
{
    Logger::d("UiHandler", "setMsgHighLight is called with msg = " + msg.toStdString());
    if (mTagHLDelegate == nullptr || mMsgHLDelegate == nullptr)
    {
        mTagHLDelegate = new HighlightDelegate(ui->table_logs);
        mMsgHLDelegate = new HighlightDelegate(ui->table_logs);
        mTagHLDelegate->setKeywords("");
        ui->table_logs->setItemDelegateForColumn(6, mTagHLDelegate);
        mMsgHLDelegate->setKeywords(msg);
        ui->table_logs->setItemDelegateForColumn(7, mMsgHLDelegate);
    } else {
        mMsgHLDelegate->setKeywords(msg);
    }
    ui->table_logs->viewport()->update();
}

void UiHandler::loadLogs(Ui::MainWindow *ui, vector<Log> logs)
{
    ui->table_logs->setRowCount(0);
    for (Log &log : logs)
    {
        const int row = ui->table_logs->rowCount();
        ui->table_logs->insertRow(row);
        ui->table_logs->setItem(row, 0, new QTableWidgetItem(QString::number(log.getLine())));
        ui->table_logs->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(log.getDate())));
        ui->table_logs->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(log.getTime())));
        ui->table_logs->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(log.getTid())));
        ui->table_logs->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(log.getPid())));
        ui->table_logs->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(log.getLevel())));
        ui->table_logs->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(log.getTag())));
        ui->table_logs->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(log.getMsg())));
    }
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
        ui->table_logmark->setSortingEnabled(false);
        ui->table_logmark->insertRow(rows);
        QTableWidgetItem *itemLine = new QTableWidgetItem();
        itemLine->setData(Qt::DisplayRole, log.getLine());
        ui->table_logmark->setItem(rows, 0, itemLine);
        ui->table_logmark->setItem(rows, 1, new QTableWidgetItem(QString::fromStdString(log.getPid())));
        ui->table_logmark->setItem(rows, 2, new QTableWidgetItem(QString::fromStdString(log.getTag())));
        ui->table_logmark->setItem(rows, 3, new QTableWidgetItem(QString::fromStdString(log.getMsg())));
        ui->table_logmark->setSortingEnabled(true);

        // sort table logmark by line after insert
        ui->table_logmark->sortItems(0, Qt::SortOrder::AscendingOrder);

        // set foreground and background color of log in table logs
        setForegroundRow(ui->table_logs, Qt::blue, row);
        setBackgroundRow(ui->table_logs, Qt::lightGray, row);
        Logger::d("UiHandler", "Mark line " + std::to_string(log.getLine()));
    }
    else
    {
        // unmark log from table
        for (int row = 0; row < rows; row++)
        {
            QTableWidgetItem *itemLine = ui->table_logmark->item(row, 0);
            if (itemLine && itemLine->data(Qt::DisplayRole).toInt() == log.getLine())
            {
                // remove row from table logmark
                ui->table_logmark->removeRow(row);

                // reset foreground and background color of log in table logs
                for (int col = 0; col < ui->table_logs->columnCount(); col++)
                {
                    ui->table_logs->item(log.getLine() - 1, col)->setBackground(QBrush());
                    ui->table_logs->item(log.getLine() - 1, col)->setForeground(QBrush());
                }
                Logger::d("UiHandler", "Unmark line " + std::to_string(log.getLine()));
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
    Logger::d("UiHandler", "targetRow = " + std::to_string(targetRow));

    // If the target row is hidden, scroll to the top
    if (ui->table_logs->isRowHidden(targetRow))
    {
        ui->table_logs->verticalScrollBar()->setValue(0);
        return;
    }

    // Scroll to the target row and set it as current
    QTableWidgetItem *itemFocus = ui->table_logs->item(targetRow, 0);
    QModelIndex modelIndex = ui->table_logs->indexFromItem(itemFocus);
    ui->table_logs->scrollTo(modelIndex, QAbstractItemView::PositionAtCenter);
    ui->table_logs->setCurrentCell(targetRow, 0);
}

void UiHandler::initUi(Ui::MainWindow *ui)
{
    ui->file->setText("C:\\Users\\ttnguyen4\\Downloads\\output_test.log");

    // Table logs
    ui->table_logs->setColumnCount(8);
    QStringList logHeaders = {"Line", "Date", "Time", "TID", "PID", "Level", "Tag", "Msg"};
    ui->table_logs->setHorizontalHeaderLabels(logHeaders);
    ui->table_logs->verticalHeader()->setVisible(false);
    ui->table_logs->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_logs->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->table_logs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_logs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_logs->setSortingEnabled(false);
    ui->table_logs->setMouseTracking(false);
    ui->table_logs->setStyleSheet(R"(
        QTableWidget::item:selected {
            background-color: #3399ff;
            color: white;
        }
    )");

    // Table logs mark
    ui->table_logmark->setColumnCount(4);
    QStringList markHeaders = {"Line", "PID", "Tag", "Msg"};
    ui->table_logmark->setHorizontalHeaderLabels(markHeaders);
    ui->table_logmark->verticalHeader()->setVisible(false);
    ui->table_logmark->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_logmark->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->table_logmark->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_logmark->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_logmark->setSortingEnabled(true);
    ui->table_logmark->setMouseTracking(false);

    // Line Text
    ui->file->setFixedHeight(LINE_HEIGTH);
    ui->pid->setFixedHeight(LINE_HEIGTH);
    ui->tag->setFixedHeight(LINE_HEIGTH);
    ui->msg->setFixedHeight(LINE_HEIGTH);
    ui->level->setFixedHeight(LINE_HEIGTH);

    // Log
    ui->log->setFixedHeight(LINE_HEIGTH);
    ui->log->setReadOnly(true);
}

void UiHandler::setBackgroundRow(QTableWidget *table, QBrush color, int row)
{
    if (row < 0 || row >= table->rowCount())
    {
        return;
    }
    for (int col = 0; col < table->columnCount(); ++col)
    {
        if (QTableWidgetItem *item = table->item(row, col))
        {
            item->setBackground(color);
        }
    }
}

void UiHandler::setForegroundRow(QTableWidget *table, QBrush color, int row)
{
    if (row < 0 || row >= table->rowCount())
    {
        return;
    }
    for (int col = 0; col < table->columnCount(); ++col)
    {
        if (QTableWidgetItem *item = table->item(row, col))
        {
            item->setForeground(color);
        }
    }
}
