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
#include <QLineEdit>
#include <QPushButton>
#include "QPushButtonDefault.h"
#include "QLineEditDefault.h"
#include <QObject>
#include "UtilHelper.hpp"
#include "SettingHelper.hpp"
#include "PropertyHelper.hpp"

const QString UiHandler::TAG = "UiHandler";

// =================== Highlight implementation ====================
// =================================================================
void UiHandler::highlightFind(const QString key)
{
    Logger::d(TAG, "highlightFindKey key = " + key);
    QStringList keys = UtilHelper::splitKeywords(key);
    mTagHLDelegate->setKeyFind(keys);
    mMsgHLDelegate->setKeyFind(keys);
    mUi->table_logs->viewport()->update();
}

void UiHandler::highlightTag(const QString &tag)
{
    Logger::d(TAG, "setTagHighLight is called with tag = " + tag);
    if (mTagHLDelegate == nullptr)
    {
        Logger::e(TAG, "mTagHLDelegate is null");
        return;
    }
    mTagHLDelegate->setKeyWords(UtilHelper::splitKeywords(tag));
    mUi->table_logs->viewport()->update();
}

void UiHandler::highlightMsg(const QString &msg)
{
    Logger::d(TAG, "setMsgHighLight is called with msg = " + msg);
    if (mMsgHLDelegate == nullptr)
    {
        Logger::e(TAG, "mMsgHLDelegate is null");
        return;
    }
    mMsgHLDelegate->setKeyWords(UtilHelper::splitKeywords(msg));
    mUi->table_logs->viewport()->update();
}

// ==================== Log implementation ====================
// ============================================================
void UiHandler::loadLogs()
{
    Logger::d(TAG, "loadLogs");
    mUi->table_logs->setUpdatesEnabled(false);
    mUi->table_logs->setRowCount(0);

    QList<Log> logs = LogHelper::getInstance()->mListLogs;
    for (int i = 0; i < logs.size(); ++i)
    {
        const Log &log = logs[i];
        insertLogToTable(log);
    }
    mUi->table_logs->setUpdatesEnabled(true);
}

void UiHandler::updateLogVisibility()
{
    QList<Log> &logs = LogHelper::getInstance()->mListLogs;
    Logger::d(TAG, "updateLogVisibility");
    mUi->table_logs->setUpdatesEnabled(false);
    for (const Log &log : logs)
    {
        int row = log.getLine() - 1;
        if (row >= 0 && row < mUi->table_logs->rowCount())
        {
            mUi->table_logs->setRowHidden(row, log.getHidden());
        }
    }
    mUi->table_logs->setUpdatesEnabled(true);
}

void UiHandler::insertLogToTable(Log log)
{
    const int row = mUi->table_logs->rowCount();
    mUi->table_logs->insertRow(row);
    mUi->table_logs->setItem(row, Constant::TableLog::COL_LINE, UtilHelper::createTableItem(QString::number(log.getLine()), Qt::AlignCenter));
    mUi->table_logs->setItem(row, Constant::TableLog::COL_DATE, UtilHelper::createTableItem(log.getDate(), Qt::AlignCenter));
    mUi->table_logs->setItem(row, Constant::TableLog::COL_TIME, UtilHelper::createTableItem(log.getTime(), Qt::AlignCenter));
    mUi->table_logs->setItem(row, Constant::TableLog::COL_PID, UtilHelper::createTableItem(log.getPid(), Qt::AlignCenter));
    mUi->table_logs->setItem(row, Constant::TableLog::COL_TID, UtilHelper::createTableItem(log.getTid(), Qt::AlignCenter));
    mUi->table_logs->setItem(row, Constant::TableLog::COL_LEVEL, UtilHelper::createTableItem(log.getLevel(), Qt::AlignCenter));
    mUi->table_logs->setItem(row, Constant::TableLog::COL_TAG, UtilHelper::createTableItem(log.getTag(), Qt::AlignLeft | Qt::AlignVCenter));
    mUi->table_logs->setItem(row, Constant::TableLog::COL_MSG, UtilHelper::createTableItem(log.getMsg(), Qt::AlignLeft | Qt::AlignVCenter));
    mUi->table_logs->setRowHidden(row, log.getHidden());
    mUi->table_logs->scrollToBottom();
}

void UiHandler::updateLogShow(QTableWidgetItem *item)
{
    mUi->log->setText(item->text());
}

void UiHandler::markLog(QTableWidgetItem *item)
{
    const int row = item->row();
    const bool isMarked = LogHelper::getInstance()->mListLogs[row].revertIsMarked();
    const int rows = mUi->table_logmark->rowCount();
    const Log log = LogHelper::getInstance()->mListLogs[row];

    if (isMarked)
    {
        // mark log to table
        mUi->table_logmark->insertRow(rows);
        QTableWidgetItem *itemLine = new QTableWidgetItem();
        itemLine->setData(Qt::DisplayRole, log.getLine());

        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_LINE, itemLine);
        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_TIME, UtilHelper::createTableItem(log.getTime(), Qt::AlignCenter));
        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_PID, UtilHelper::createTableItem(log.getPid(), Qt::AlignCenter));
        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_TAG, UtilHelper::createTableItem(log.getTag(), Qt::AlignLeft | Qt::AlignVCenter));
        mUi->table_logmark->setItem(rows, Constant::TableLogMark::COL_MSG, UtilHelper::createTableItem(log.getMsg(), Qt::AlignLeft | Qt::AlignVCenter));

        mUi->table_logmark->setSortingEnabled(true); // enable sorting to sort table logmark
        mUi->table_logmark->sortItems(Constant::TableLogMark::COL_LINE, Qt::SortOrder::AscendingOrder);
        mUi->table_logmark->setSortingEnabled(false); // disable sorting after completing sorting

        // set foreground and background color of log in table logs
        setHighLightMarkRow(mUi->table_logs, row, MARK_LOG_FOREGROUND_COLOR, MARK_LOG_BACKGROUND_COLOR);
        Logger::d(TAG, "Mark line " + QString::number(log.getLine()));
        return;
    }

    // unmark log from table log mark
    for (int row = 0; row < rows; row++)
    {
        QTableWidgetItem *itemLine = mUi->table_logmark->item(row, Constant::TableLogMark::COL_LINE);
        if (itemLine && itemLine->data(Qt::DisplayRole).toInt() == log.getLine())
        {
            mUi->table_logmark->removeRow(row);                      // remove row from table logmark
            setHighLightMarkRow(mUi->table_logs, log.getLine() - 1); // reset foreground and background color of log in table logs
            Logger::d(TAG, "Unmark line " + QString::number(log.getLine()));
            break;
        }
    }
}

void UiHandler::focusLog(QTableWidgetItem *item)
{
    // Get the line number from the selected item in table_logmark
    QTableWidgetItem *itemLine = mUi->table_logmark->item(item->row(), Constant::TableLogMark::COL_LINE);
    if (!itemLine)
        return;

    int line = itemLine->data(Qt::DisplayRole).toInt();
    int targetRow = line - 1;

    // Find the nearest visible row at or before targetRow
    while (targetRow >= 0 && mUi->table_logs->isRowHidden(targetRow))
        --targetRow;

    Logger::d(TAG, "focusLog: targetRow = " + QString::number(targetRow));

    if (targetRow < 0 || mUi->table_logs->isRowHidden(targetRow))
    {
        // No visible row found, scroll to top
        mUi->table_logs->verticalScrollBar()->setValue(0);
        return;
    }

    // Focus the row in table_logs
    mUi->table_logs->setCurrentCell(targetRow, Constant::TableLog::COL_LINE);
    QModelIndex modelIndex = mUi->table_logs->model()->index(targetRow, Constant::TableLog::COL_LINE);
    mUi->table_logs->scrollTo(modelIndex, QAbstractItemView::PositionAtCenter);
}

// ==================== Setting implementation ====================
// ================================================================
void UiHandler::loadSettings()
{
    QList<Setting> settings = SettingHelper::getInstance()->mListSettings;
    mUi->table_setting->setUpdatesEnabled(false);
    mUi->table_setting->setRowCount(settings.size());
    for (int i = 0; i < settings.size(); ++i)
    {
        insertSettingToTable(settings[i], i);
    }
    mUi->table_setting->setUpdatesEnabled(true);
}

void UiHandler::updateValueSettings()
{
    const QList<Setting> settings = SettingHelper::getInstance()->mListSettings;
    mUi->table_setting->setUpdatesEnabled(false);
    for (int i = 0; i < settings.size(); ++i)
    {
        const Setting &setting = settings[i];
        QList<QTableWidgetItem *> itemNames = mUi->table_setting->findItems(setting.getName(), Qt::MatchExactly);
        if (!itemNames.isEmpty())
        {
            QTableWidgetItem *itemName = itemNames.first();
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(mUi->table_setting->cellWidget(itemName->row(), Constant::TableSetting::COL_VALUE));
            if (lineEdit)
            {
                lineEdit->setText(setting.getValue());
            }
        }
    }
    mUi->table_setting->setUpdatesEnabled(true);
}

void UiHandler::updateSettingsVisibility()
{
    const QList<Setting> settings = SettingHelper::getInstance()->mListSettings;
    Logger::d(TAG, "updateSettingsVisibility");
    mUi->table_setting->setUpdatesEnabled(false);
    for (const Setting &setting : settings)
    {
        Logger::d(TAG, "setting: " + QString::number(setting.getIsHidden()));
        mUi->table_setting->setRowHidden(setting.getLine() - 1, setting.getIsHidden());
    }
    mUi->table_setting->setUpdatesEnabled(true);
}

void UiHandler::insertSettingToTable(const Setting setting, const int row)
{
    mUi->table_setting->setItem(row, Constant::TableSetting::COL_GROUP, UtilHelper::createTableItem(setting.getGroup(), Qt::AlignCenter));
    mUi->table_setting->setItem(row, Constant::TableSetting::COL_SETTING, UtilHelper::createTableItem(setting.getName(), Qt::AlignLeft | Qt::AlignVCenter));
    mUi->table_setting->setCellWidget(row, Constant::TableSetting::COL_VALUE, new QLineEditDefault(setting.getValue()));

    QPushButtonDefault *save = new QPushButtonDefault("");
    QObject::connect(save, &QPushButtonDefault::pressed, [this, row]()
                     {
                const QString deviceId = mUi->device_ids->currentText();
                if (deviceId.isEmpty())
                    return;

                auto *itemGroup = mUi->table_setting->item(row, Constant::TableSetting::COL_GROUP);
                auto *itemSetting = mUi->table_setting->item(row, Constant::TableSetting::COL_SETTING);
                auto *valueEdit = qobject_cast<QLineEdit*>(mUi->table_setting->cellWidget(row, Constant::TableSetting::COL_VALUE));

                if (!itemGroup || !itemSetting || !valueEdit)
                    return;

                const QString group = itemGroup->text();
                const QString name = itemSetting->text();
                const QString value = valueEdit->text();

                Logger::d(TAG, QString("Save setting: group: %1, setting: %2, value: %3").arg(group, name, value));
                SettingHelper::getInstance()->setSetting(Setting(group, name, value), deviceId); });
    mUi->table_setting->setCellWidget(row, Constant::TableSetting::COL_SEND, save);
    mUi->table_setting->setRowHidden(row, setting.getIsHidden());
}

// ==================== Property implementation ====================
// =================================================================
void UiHandler::loadProperties()
{
    QList<Property> properties = PropertyHelper::getInstance()->mListProperties;
    mUi->table_property->setUpdatesEnabled(false);
    mUi->table_property->setRowCount(properties.size());
    for (int i = 0; i < properties.size(); ++i)
    {
        insertPropertyToTable(properties[i], i);
    }
    mUi->table_property->setUpdatesEnabled(true);
}

void UiHandler::updateValueProperties()
{
    const QList<Property> properties = PropertyHelper::getInstance()->mListProperties;
    mUi->table_property->setUpdatesEnabled(false);
    for (int i = 0; i < properties.size(); ++i)
    {
        const Property &property = properties[i];
        QList<QTableWidgetItem *> itemNames = mUi->table_property->findItems(property.getName(), Qt::MatchExactly);
        if (!itemNames.isEmpty())
        {
            QTableWidgetItem *itemName = itemNames.first();
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(mUi->table_property->cellWidget(itemName->row(), Constant::TableProperty::COL_VALUE));
            if (lineEdit)
            {
                lineEdit->setText(property.getValue());
            }
        }
    }
    mUi->table_property->setUpdatesEnabled(true);
}

void UiHandler::insertPropertyToTable(const Property property, const int row)
{
    mUi->table_property->setItem(row, Constant::TableProperty::COL_PROPERTY, UtilHelper::createTableItem(property.getName(), Qt::AlignLeft | Qt::AlignVCenter));
    mUi->table_property->setCellWidget(row, Constant::TableProperty::COL_VALUE, new QLineEditDefault(property.getValue()));
    QPushButtonDefault *save = new QPushButtonDefault("");
    QObject::connect(save, &QPushButtonDefault::pressed, [this, row]() {
                const QString deviceId = mUi->device_ids->currentText();
                if (deviceId.isEmpty())
                {
                    return;
                }

                auto *itemProperty = mUi->table_property->item(row, Constant::TableProperty::COL_PROPERTY);
                auto *valueEdit = qobject_cast<QLineEdit*>(mUi->table_property->cellWidget(row, Constant::TableProperty::COL_VALUE));

                if (!itemProperty || !valueEdit)
                    return;

                const QString name = itemProperty->text();
                const QString value = valueEdit->text();

                Logger::d(TAG, QString("Save property: property: %1, value: %2").arg(name, value));
                PropertyHelper::getInstance()->setProperty(Property(name, value), deviceId); });
    mUi->table_property->setCellWidget(row, Constant::TableProperty::COL_SEND, save);
    mUi->table_property->setRowHidden(row, property.getIsHidden());
}

void UiHandler::updatePropertiesVisibility()
{
    Logger::d(TAG, "updatePropertiesVisibility");
    const QList<Property> properties = PropertyHelper::getInstance()->mListProperties;
    mUi->table_property->setUpdatesEnabled(false);
    for (const Property &property : properties)
    {
        mUi->table_property->setRowHidden(property.getLine() - 1, property.getIsHidden());
    }
    mUi->table_property->setUpdatesEnabled(true);
}

// ==================== UI Initial implementation ====================
// ===================================================================
void UiHandler::initUi(Ui::MainWindow *ui)
{
    mUi = ui;
    // mUi->file->setText("C:\\Users\\ttnguyen4\\Downloads\\output.log");
    mUi->file->setText("/home/truongnguyen/Downloads/output.log");

    // Table logs
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
    mUi->table_logmark->setStyleSheet(R"(
        QTableWidget::item:selected {
            background-color: #3399ff;
            color: white;
        }
    )");

    // Table properties
    mUi->table_property->verticalHeader()->setVisible(false);
    mUi->table_property->setWordWrap(false);
    mUi->table_property->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mUi->table_property->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->table_property->setSortingEnabled(false);
    mUi->table_property->setMouseTracking(false);
    mUi->table_property->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mUi->table_property->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mUi->table_property->setAutoScroll(false);
    mUi->table_property->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mUi->table_property->horizontalHeader()->setSectionResizeMode(Constant::TableProperty::COL_VALUE, QHeaderView::Stretch);
    mUi->table_property->setStyleSheet(R"(
            QTableWidget::item:selected {
                background-color: #3399ff;
                color: white;
            }
        )");

    // Table settings
    mUi->table_setting->verticalHeader()->setVisible(false);
    mUi->table_setting->setWordWrap(false);
    mUi->table_setting->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mUi->table_setting->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->table_setting->setSortingEnabled(false);
    mUi->table_setting->setMouseTracking(false);
    mUi->table_setting->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mUi->table_setting->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mUi->table_setting->setAutoScroll(false);
    mUi->table_setting->resizeColumnsToContents();
    mUi->table_setting->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mUi->table_setting->horizontalHeader()->setSectionResizeMode(Constant::TableSetting::COL_VALUE, QHeaderView::Stretch);
    mUi->table_setting->setStyleSheet(R"(
        QTableWidget::item:selected {
            background-color: #3399ff;
            color: white;
        }
    )");

    // Line Text
    mUi->file->setFixedHeight(LINE_HEIGTH);
    mUi->pid->setFixedHeight(LINE_HEIGTH);
    mUi->tag->setFixedHeight(LINE_HEIGTH);
    mUi->msg->setFixedHeight(LINE_HEIGTH);
    mUi->level->setFixedHeight(LINE_HEIGTH);
    mUi->find->setFixedHeight(LINE_HEIGTH);

    // Log
    mUi->log->setFixedHeight(LINE_HEIGTH * 2);
    mUi->log->setReadOnly(true);

    initHLDelegate();
}

// ==================== Other UI implementation ====================
// =================================================================
void UiHandler::clearLogcat()
{
    mUi->table_logmark->clearContents();
    mUi->table_logs->clearContents();
    mUi->log->clear();
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
            LogHelper::getInstance()->mListLogs[row].revertIsMarked();
        }
    }
    mUi->table_logmark->setRowCount(0);
}

void UiHandler::refreshDeviceIds(QStringList deviceIds, const bool isConnected)
{
    mUi->device_ids->setUpdatesEnabled(false);
    if (isConnected)
    {
        for (const QString &deviceId : deviceIds)
        {
            if (mUi->device_ids->findText(deviceId) == -1)
            {
                mUi->device_ids->addItem(deviceId);
            }
        }
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
    mUi->device_ids->setUpdatesEnabled(true);
    if (mUi->device_ids->currentText().isEmpty())
    {
        mUi->device_ids->setCurrentText(mUi->device_ids->count() > 0 ? mUi->device_ids->itemText(0) : "");
    }
    mUi->devices->setText("[" + QString::number(mUi->device_ids->count()) + "]");
}

void UiHandler::startWatching(const bool isWatching)
{
    if (isWatching)
    {
        mUi->table_logs->clearContents();
    }
    mUi->start->setIcon(isWatching ? QIcon(":/icons/stop.png") : QIcon(":/icons/play.png"));
    mUi->clear->setDisabled(isWatching);
}

//===========================================================
//============== Private methods ============================
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
        mUi->table_logs->setItemDelegateForColumn(Constant::TableLog::COL_TAG, mTagHLDelegate);
    }
    if (mMsgHLDelegate == nullptr)
    {
        mMsgHLDelegate = HighlightKey::Builder(mUi->table_logs)
                             .setBold(true)
                             .build();
        mUi->table_logs->setItemDelegateForColumn(Constant::TableLog::COL_MSG, mMsgHLDelegate);
    }
    if (mLevelHLDelegate == nullptr)
    {
        mLevelHLDelegate = HighlightCell::Builder(mUi->table_logs)
                               .setBold(true)
                               .build();
        mLevelHLDelegate->setKeyWords({Constant::LogLevel::V,
                                       Constant::LogLevel::D,
                                       Constant::LogLevel::I,
                                       Constant::LogLevel::W,
                                       Constant::LogLevel::E});
        mUi->table_logs->setItemDelegateForColumn(Constant::TableLog::COL_LEVEL, mLevelHLDelegate);
    }
}
