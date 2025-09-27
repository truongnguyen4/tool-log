#include "NotificationManager.hpp"
#include "NotificationDialog.hpp"
#include "mainwindow.h"

const QString NotificationManager::TAG = "NotificationManager";

void NotificationManager::showError(const int &errorCode)
{
    if (errorCode != MainWindow::SUCCESS)
    {
        QString msg;
        switch (errorCode)
        {
            case MainWindow::ERROR_DEVICE_ID:
                msg = "Device ID is not available, please refresh list devices";
                break;
            case MainWindow::ERROR_FILE_PATH:
                msg = "File path is not available, please check your file path";
                break;
            default:
                break;
        }
        NotificationDialog::show(NotificationDialog::ERROR, msg, TAG);
    }
}
