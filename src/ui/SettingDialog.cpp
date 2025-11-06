#include "SettingDialog.hpp"
#include "ui_Settings.h"
#include "Logger.hpp"
#include "Constant.hpp"

const QString SettingDialog::TAG = "SettingDialog";

SettingDialog::SettingDialog(QWidget* parent, Ui::MainWindow *ui_main)
    : QDialog(parent), ui(new Ui::Settings), ui_main(ui_main)
{
    ui->setupUi(this);
    adjustSize();
    init();
    onSettings();
    setWindowTitle("Settings");
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::onSettings()
{
    Logger::d(TAG, "onSettings is called");

    ui->cb_line->setChecked(true);
    if (ui_main != nullptr)
    {
        ui->cb_line->setChecked(!ui_main->table_logs->isColumnHidden(Constant::TableLog::COL_LINE));
        ui->cb_date->setChecked(!ui_main->table_logs->isColumnHidden(Constant::TableLog::COL_DATE));
        ui->cb_time->setChecked(!ui_main->table_logs->isColumnHidden(Constant::TableLog::COL_TIME));
        ui->cb_tid->setChecked(!ui_main->table_logs->isColumnHidden(Constant::TableLog::COL_TID));
        ui->cb_pid->setChecked(!ui_main->table_logs->isColumnHidden(Constant::TableLog::COL_PID));
        ui->cb_level->setChecked(!ui_main->table_logs->isColumnHidden(Constant::TableLog::COL_LEVEL));
        ui->cb_tag->setChecked(!ui_main->table_logs->isColumnHidden(Constant::TableLog::COL_TAG));
        ui->cb_msg->setChecked(!ui_main->table_logs->isColumnHidden(Constant::TableLog::COL_MSG));
    }
}

void SettingDialog::onAccepted()
{
    if (ui_main != nullptr)
    {
        ui_main->table_logs->setColumnHidden(Constant::TableLog::COL_LINE, !ui->cb_line->isChecked());
        ui_main->table_logs->setColumnHidden(Constant::TableLog::COL_DATE, !ui->cb_date->isChecked());
        ui_main->table_logs->setColumnHidden(Constant::TableLog::COL_TIME, !ui->cb_time->isChecked());
        ui_main->table_logs->setColumnHidden(Constant::TableLog::COL_TID, !ui->cb_tid->isChecked());
        ui_main->table_logs->setColumnHidden(Constant::TableLog::COL_PID, !ui->cb_pid->isChecked());
        ui_main->table_logs->setColumnHidden(Constant::TableLog::COL_LEVEL, !ui->cb_level->isChecked());
        ui_main->table_logs->setColumnHidden(Constant::TableLog::COL_TAG, !ui->cb_tag->isChecked());
        ui_main->table_logs->setColumnHidden(Constant::TableLog::COL_MSG, !ui->cb_msg->isChecked());

        ui_main->table_logmark->setColumnHidden(Constant::TableLog::COL_LINE, !ui->cb_line->isChecked());
        ui_main->table_logmark->setColumnHidden(Constant::TableLog::COL_DATE, !ui->cb_date->isChecked());
        ui_main->table_logmark->setColumnHidden(Constant::TableLog::COL_TIME, !ui->cb_time->isChecked());
        ui_main->table_logmark->setColumnHidden(Constant::TableLog::COL_TID, !ui->cb_tid->isChecked());
        ui_main->table_logmark->setColumnHidden(Constant::TableLog::COL_PID, !ui->cb_pid->isChecked());
        ui_main->table_logmark->setColumnHidden(Constant::TableLog::COL_LEVEL, !ui->cb_level->isChecked());
        ui_main->table_logmark->setColumnHidden(Constant::TableLog::COL_TAG, !ui->cb_tag->isChecked());
        ui_main->table_logmark->setColumnHidden(Constant::TableLog::COL_MSG, !ui->cb_msg->isChecked());
    }
}

void SettingDialog::init()
{
    connect(ui->dialog_btn, &QDialogButtonBox::accepted, this, &SettingDialog::onAccepted);
}


