#ifndef QPUSHBUTTONDEFAULT_H
#define QPUSHBUTTONDEFAULT_H
#include <QPushButton>
class QPushButtonDefault : public QPushButton
{
public:
    QPushButtonDefault(const QString &text, QWidget *parent = nullptr) : QPushButton(text, parent) {
        setFocusPolicy(Qt::NoFocus);
        setAutoDefault(false);
        setDefault(false);
        setIcon(QIcon(":/icons/save.png"));
    }
};

#endif // QPUSHBUTTONDEFAULT_H
