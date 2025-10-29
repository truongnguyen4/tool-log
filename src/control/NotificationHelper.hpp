#ifndef NOTIFICATIONHELPER_H
#define NOTIFICATIONHELPER_H
#include <QString>
#include "NotificationDialog.hpp"

class NotificationHelper
{
private:
    inline static const QString TAG = "NotificationHelper";

public:
    static const int ERROR_DEVICE_ID = 0;
    static const int ERROR_FILE_PATH = 1;
    static const int ERROR_PROCESS_SHELL = 2;
    static void showError(const int errorCode)
    {
        QString msg;
        switch (errorCode)
        {
        case ERROR_DEVICE_ID:
            msg = "Device ID is not available, please refresh list devices";
            break;
        case ERROR_FILE_PATH:
            msg = "File path is not available, please check your file path";
            break;
        case ERROR_PROCESS_SHELL:
            msg = "Can't perform shell command, please check your adb connection";
            break;
        default:
            break;
        }
        NotificationDialog::show(NotificationDialog::ERROR, msg, TAG);
    }
};

#endif // NOTIFICATIONHELPER_H
