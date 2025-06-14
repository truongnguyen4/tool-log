#ifndef NOTIFICATIONDIALOG_HPP
#define NOTIFICATIONDIALOG_HPP
#include <QDialog>

class NotificationDialog : public QDialog {
    Q_OBJECT
public:
    static QString WARNING;
    static QString ERROR;
    static QString INFO;
    explicit NotificationDialog(QWidget* parent = nullptr);
    static void show(QString type, QString message, QString TAG);
private:
    NotificationDialog(QString type, QString message);
};

#endif // NOTIFICATIONDIALOG_HPP
