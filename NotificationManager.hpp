#ifndef NOTIFICATIONMANAGER_HPP
#define NOTIFICATIONMANAGER_HPP
#include <QString>

class NotificationManager
{
private:
    static const QString TAG;
public:
    static void showError(const int &errorCode);
};


#endif // NOTIFICATIONMANAGER_HPP