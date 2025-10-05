#include "UiHandler.hpp"
#include <QTableWidgetItem>
#include "Logger.hpp"
#include <QScrollBar>
#include "Constant.hpp"
#include "mainwindow.h"
#include <QDateTime>
#include "LogHelper.hpp"
#include "HighlightKey.hpp"
#include "HighlightCell.hpp"

const QString UiHandler::TAG = "UiHandler";
//============== Public methods ==============

void UiHandler::highlightFindKey(const QString key)
{
    Logger::d(TAG, "highlightFindKey key = " + key);
    mTagHLDelegate->setKeyFind(key);
    mMsgHLDelegate->setKeyFind(key);
    mUi->table_logs->viewport()->update();
}

void UiHandler::setTagHighLight(const QString &tag)
{
    Logger::d(TAG, "setTagHighLight is called with tag = " + tag);
    if (mTagHLDelegate == nullptr)
    {
        Logger::e(TAG, "mTagHLDelegate is null");
        return;
    }
    mTagHLDelegate->setKeyWords(LogHelper::splitKeywords(tag));
    mUi->table_logs->viewport()->update();
}

void UiHandler::setMsgHighLight(const QString &msg)
{
    Logger::d(TAG,"setMsgHighLight is called with msg = " + msg);
    if (mMsgHLDelegate == nullptr)
    {
        Logger::e(TAG, "mMsgHLDelegate is null");
        return;
    }
    mMsgHLDelegate->setKeyWords(LogHelper::splitKeywords(msg));
    mUi->table_logs->viewport()->update();
}

void UiHandler::loadLogs(QList<Log> logs)
{
    Logger::d(TAG, "Load log with number of logs: " + QString::number(logs.size()));
    mUi->table_logs->setRowCount(0);
    mUi->table_logs->setUpdatesEnabled(false);
    for (const Log &log : logs)
    {
        const int row = mUi->table_logs->rowCount();
        mUi->table_logs->insertRow(row);

        QTableWidgetItem *lineItem = new QTableWidgetItem(QString::number(log.getLine()));
        lineItem->setTextAlignment(Qt::AlignCenter);
        mUi->table_logs->setItem(row, Constant::TableLog::COL_LINE, lineItem);

        QTableWidgetItem *dateItem = new QTableWidgetItem(log.getDate());
        dateItem->setTextAlignment(Qt::AlignCenter);
        mUi->table_logs->setItem(row, Constant::TableLog::COL_DATE, dateItem);

        QTableWidgetItem *timeItem = new QTableWidgetItem(log.getTime());
        timeItem->setTextAlignment(Qt::AlignCenter);
        mUi->table_logs->setItem(row, Constant::TableLog::COL_TIME, timeItem);

        QTableWidgetItem *pidItem = new QTableWidgetItem(log.getPid());
        pidItem->setTextAlignment(Qt::AlignCenter);
        mUi->table_logs->setItem(row, Constant::TableLog::COL_PID, pidItem);

        QTableWidgetItem *tidItem = new QTableWidgetItem(log.getPid());
        tidItem->setTextAlignment(Qt::AlignCenter);
        mUi->table_logs->setItem(row, Constant::TableLog::COL_TID, tidItem);

        QTableWidgetItem *levelItem = new QTableWidgetItem(log.getLevel());
        levelItem->setTextAlignment(Qt::AlignCenter);
        mUi->table_logs->setItem(row, Constant::TableLog::COL_LEVEL, levelItem);

        mUi->table_logs->setItem(row, Constant::TableLog::COL_TAG, new QTableWidgetItem(log.getTag()));
        
        mUi->table_logs->setItem(row, Constant::TableLog::COL_MSG, new QTableWidgetItem(log.getMsg()));
    }
    mUi->table_logs->setUpdatesEnabled(true);
}

void UiHandler::updateLogShow(QTableWidgetItem *item)
{
    mUi->log->setText(item->text());
}

void UiHandler::markLog(QTableWidgetItem *item)
{
    const int row = item->row();
    const bool isMarked = LogHelper::mListLogs[row].revertIsMarked();
    const int rows = mUi->table_logmark->rowCount();
    const Log log = LogHelper::mListLogs[row];

    if (isMarked)
    {
        // mark log to table
        mUi->table_logmark->insertRow(rows);
        QTableWidgetItem *itemLine = new QTableWidgetItem();
        itemLine->setData(Qt::DisplayRole, log.getLine());
        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_LINE, itemLine);
        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_PID, new QTableWidgetItem(log.getPid()));
        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_TAG, new QTableWidgetItem(log.getTag()));
        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_MSG, new QTableWidgetItem(log.getMsg()));

        mUi->table_logmark->setSortingEnabled(true);   // enable sorting to sort table logmark
        mUi->table_logmark->sortItems(Constant::TableLogMark::COL_LINE, Qt::SortOrder::AscendingOrder);
        mUi->table_logmark->setSortingEnabled(false);  // disable sorting after completing sorting

        // set foreground and background color of log in table logs
        setHighLightMarkRow(mUi->table_logs, row, MARK_LOG_FOREGROUND_COLOR, MARK_LOG_BACKGROUND_COLOR);
        Logger::d(TAG, "Mark line " + QString::number(log.getLine()));
    }
    else
    {
        // unmark log from table log mark
        for (int row = 0; row < rows; row++)
        {
            QTableWidgetItem *itemLine = mUi->table_logmark->item(row, Constant::TableLogMark::COL_LINE);
            if (itemLine && itemLine->data(Qt::DisplayRole).toInt() == log.getLine())
            {
                mUi->table_logmark->removeRow(row);   // remove row from table logmark
                setHighLightMarkRow(mUi->table_logs, log.getLine()-1);  // reset foreground and background color of log in table logs
                Logger::d(TAG, "Unmark line " + QString::number(log.getLine()));
                break;
            }
        }
    }
}

void UiHandler::focusLog(QTableWidgetItem *item)
{
    int line = mUi->table_logmark->item(item->row(), 0)->data(Qt::DisplayRole).toInt();
    int targetRow = line - 1;

    // Ensure the target row is not hidden
    while (targetRow > 0 && mUi->table_logs->isRowHidden(targetRow))
    {
        targetRow -= 1;
    }
    Logger::d(TAG, "targetRow = " +QString::number(targetRow));

    // If the target row is hidden, scroll to the top
    if (mUi->table_logs->isRowHidden(targetRow))
    {
        mUi->table_logs->verticalScrollBar()->setValue(0);
        return;
    }

    // Scroll to the target row and set it as current
    QTableWidgetItem *itemFocus = mUi->table_logs->item(targetRow, Constant::TableLog::COL_LINE);
    QModelIndex modelIndex = mUi->table_logs->indexFromItem(itemFocus);
    mUi->table_logs->scrollTo(modelIndex, QAbstractItemView::PositionAtCenter);
    mUi->table_logs->setCurrentCell(targetRow, Constant::TableLog::COL_LEVEL);
}

void UiHandler::initUi(Ui::MainWindow *ui)
{
    mUi = ui;
    mUi->file->setText("C:\\Users\\ttnguyen4\\Downloads\\output.log");
    // mUi->file->setText("/home/truongnguyen/Downloads/output.log");
    // Table logs
    mUi->table_logs->setColumnCount(8);
    QStringList logHeaders = { Constant::TableLog::LINE,
                                Constant::TableLog::DATE,
                                Constant::TableLog::TIME,
                                Constant::TableLog::PID,
                                Constant::TableLog::TID,
                                Constant::TableLog::LEVEL,
                                Constant::TableLog::TAG,
                                Constant::TableLog::MSG };
    mUi->table_logs->setHorizontalHeaderLabels(logHeaders);
    mUi->table_logs->verticalHeader()->setVisible(false);
    mUi->table_logs->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mUi->table_logs->setWordWrap(false);
    mUi->table_logs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mUi->table_logs->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->table_logs->setSortingEnabled(false);
    mUi->table_logs->setMouseTracking(false);
    mUi->table_logs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mUi->table_logs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mUi->table_logs->setAutoScroll(false);
    mUi->table_logs->setColumnHidden(Constant::TableLog::COL_DATE, true);
    mUi->table_logs->setColumnHidden(Constant::TableLog::COL_TIME, true);
    mUi->table_logs->setColumnHidden(Constant::TableLog::COL_TID, true);
    mUi->table_logs->setStyleSheet(R"(
        QTableWidget::item:selected {
            background-color: #3399ff;
            color: white;
        }
    )");

    // Table logs mark
    mUi->table_logmark->setColumnCount(4);
    QStringList markHeaders = { Constant::TableLogMark::LINE,
                                Constant::TableLogMark::PID,
                                Constant::TableLogMark::TAG,
                                Constant::TableLogMark::MSG };
    mUi->table_logmark->setHorizontalHeaderLabels(markHeaders);
    mUi->table_logmark->verticalHeader()->setVisible(false);
    mUi->table_logmark->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mUi->table_logmark->setWordWrap(false);
    mUi->table_logmark->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mUi->table_logmark->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->table_logmark->setSortingEnabled(false);
    mUi->table_logmark->setMouseTracking(false);
    mUi->table_logmark->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mUi->table_logmark->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mUi->table_logmark->setAutoScroll(false);

    // Line Text
    mUi->file->setFixedHeight(LINE_HEIGTH);
    mUi->pid->setFixedHeight(LINE_HEIGTH);
    mUi->tag->setFixedHeight(LINE_HEIGTH);
    mUi->msg->setFixedHeight(LINE_HEIGTH);
    mUi->level->setFixedHeight(LINE_HEIGTH);
    mUi->find->setFixedHeight(LINE_HEIGTH);
    // Log
    mUi->log->setFixedHeight(LINE_HEIGTH*2);
    mUi->log->setReadOnly(true);

    initHLDelegate();
}

void UiHandler::updateLogVisibility(QList<Log> &logs)
{
    Logger::setTimeFrom("Set hidden", QDateTime::currentMSecsSinceEpoch());
    mUi->table_logs->setUpdatesEnabled(false);
    for (const Log &log : logs)
    {
        mUi->table_logs->setRowHidden(log.getLine() - 1, log.getHidden());
    }
    mUi->table_logs->setUpdatesEnabled(true);
    Logger::setTimeTo("Set hidden", QDateTime::currentMSecsSinceEpoch());
}

void UiHandler::clearLogcat()
{
    clearTextInput(mUi->pid, mUi->tag, mUi->msg, mUi->level);
    mUi->table_logmark->clearContents();
    mUi->table_logs->clearContents();
    mUi->log->clear();
}

void UiHandler::setLineEdit(QObject *obj, const QString &key)
{
    if (obj == mUi->pid)
    {
        mUi->pid->setText(key);
    }
    if (obj == mUi->tag)
    {
        mUi->tag->setText(key);
    }
    if (obj == mUi->msg)
    {
        mUi->msg->setText(key);
    }
    if (obj == mUi->level)
    {
        mUi->level->setText(key);
    }
}

void UiHandler::clearMarkLogs()
{
    const int rows = mUi->table_logmark->rowCount();
    for (int row = 0; row < rows; row++)
    {
        QTableWidgetItem *itemLine = mUi->table_logmark->item(row, Constant::TableLogMark::COL_LINE);
        if (itemLine)
        {
            int row = itemLine->data(Qt::DisplayRole).toInt() - 1;
            setHighLightMarkRow(mUi->table_logs, row);
            LogHelper::mListLogs[row].revertIsMarked();
        }
    }
    mUi->table_logmark->setRowCount(0);
}

void UiHandler::refreshDeviceIds(QStringList deviceIds, const bool isConnected)
{
    if (isConnected)
    {
        mUi->device_ids->addItems(deviceIds);
    }
    else
    {
        for (const QString &deviceId : deviceIds)
        {
            int index = mUi->device_ids->findText(deviceId);
            if (index != -1)
            {
                if (mUi->device_ids->currentIndex() == index)
                {
                    mUi->device_ids->setCurrentIndex(0);
                }
                mUi->device_ids->removeItem(index);
            }
        }
    }
    mUi->device_ids->update();
    mUi->devices->setText("(" + QString::number(mUi->device_ids->count()) + ")");
}

void UiHandler::startWatching(const bool isWatching)
{
    QString text = isWatching ? "Stop" : "Start";
    mUi->start->setText(text);
    mUi->clear->setDisabled(isWatching);
    setDisableTextInput(isWatching /*disable*/, mUi->file, mUi->pid, mUi->tag, mUi->msg, mUi->level);
    setDisableComboBox(isWatching /*disable*/, mUi->device_ids);
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

void UiHandler::initHLDelegate()
{
    if (mTagHLDelegate == nullptr)
    {
        mTagHLDelegate = HighlightKey::Builder(mUi->table_logs)
                                        .setBold(true)
                                        .build();
        mTagHLDelegate->setKeyWords(QStringList());
        mUi->table_logs->setItemDelegateForColumn(Constant::TableLog::COL_TAG, mTagHLDelegate);
    }
    if (mMsgHLDelegate == nullptr)
    {
        mMsgHLDelegate = HighlightKey::Builder(mUi->table_logs)
                                        .setBold(true)
                                        .build();
        mMsgHLDelegate->setKeyWords(QStringList());
        mUi->table_logs->setItemDelegateForColumn(Constant::TableLog::COL_MSG, mMsgHLDelegate);
    }
    if (mLevelHLDelegate == nullptr)
    {
        mLevelHLDelegate = HighlightCell::Builder(mUi->table_logs)
                                        .setBold(true)
                                        .build();
        mLevelHLDelegate->setKeyWords(QStringList() << "V" << "D" << "I" << "W" << "E");
        mUi->table_logs->setItemDelegateForColumn(Constant::TableLog::COL_LEVEL, mLevelHLDelegate);
    }
}
