#ifndef SETTINGDIALOG_HPP
#define SETTINGDIALOG_HPP
#include <QDialog>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Settings;
    class MainWindow;
}
QT_END_NAMESPACE

class SettingDialog : public QDialog
{
    Q_OBJECT
private:
    static const QString TAG;
    Ui::Settings *ui;
    Ui::MainWindow *ui_main;
    void onSettings();
    void onAccepted();
    void init();

public:
    explicit SettingDialog(QWidget *parent = nullptr, Ui::MainWindow *ui_main = nullptr);
    ~SettingDialog();
};

#endif // SETTINGDIALOG_HPP
