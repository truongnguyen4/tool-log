#include "NotificationDialog.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "Logger.hpp"

QString NotificationDialog::WARNING = "Warn";
QString NotificationDialog::ERROR = "Error";
QString NotificationDialog::INFO = "Info";

NotificationDialog::NotificationDialog(QWidget* parent)
    : QDialog(parent)
{
}

NotificationDialog::NotificationDialog(QString type, QString message)
{
    setWindowTitle(type);

    auto* layout = new QGridLayout(this);

    auto* label = new QLabel(message);
    auto* closeButton = new QPushButton("Close");

    auto* container = new QWidget();
    auto* vbox = new QVBoxLayout(container);
    vbox->addWidget(label, 0, Qt::AlignHCenter);
    vbox->addWidget(closeButton, 0, Qt::AlignHCenter);

    layout->addWidget(container, 0, 0, Qt::AlignCenter);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void NotificationDialog::show(QString type, QString message, QString TAG)
{
    NotificationDialog dialog(type, message);
    dialog.exec();
    if (type == WARNING)
    {
        Logger::w(TAG, message);
        return;
    }
    if (type == ERROR)
    {
        Logger::e(TAG, message);
        return;
    }
    if (type == INFO)
    {
        Logger::d(TAG, message);
        return;
    }
}
