#ifndef QLINEEDITDEFAULT_H
#define QLINEEDITDEFAULT_H
#include <QString>
#include <QLineEdit>

class QLineEditDefault : public QLineEdit
{
public:
    explicit QLineEditDefault(const QString &text = QString(), QWidget *parent = nullptr)
        : QLineEdit(text, parent)
    {
        setFocusPolicy(Qt::StrongFocus);
        setFrame(false);
        setAutoFillBackground(false);
        setStyleSheet("background:transparent; border:none;");
        setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
};


#endif // QLINEEDITDEFAULT_H
