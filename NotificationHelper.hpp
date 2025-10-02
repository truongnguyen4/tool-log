#ifndef NOTIFICATIONHELPER_H
#define NOTIFICATIONHELPER_H
#include <QString>

class NotificationHelper
{
private:
    static const QString TAG;
public:
    static void showError(const int &errorCode);
};


#endif // NOTIFICATIONHELPER_H
