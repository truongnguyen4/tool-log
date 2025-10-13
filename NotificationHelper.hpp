#ifndef NOTIFICATIONHELPER_H
#define NOTIFICATIONHELPER_H
#include <QString>
#include "mainwindow.h"

class NotificationHelper
{
private:
    inline static const QString TAG = "NotificationHelper";
public:
    static void showError(const int &errorCode)
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

    static void showExitCode()
    {
        NotificationDialog::show(NotificationDialog::ERROR, "Can't perform shell command", TAG);
    }
};


#endif // NOTIFICATIONHELPER_H
