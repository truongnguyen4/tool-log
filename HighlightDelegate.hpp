#ifndef HIGLIGHTDELEGATE_H
#define HIGLIGHTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextLayout>

class HighlightDelegate : public QStyledItemDelegate 
{
public:
    HighlightDelegate(QObject* parent = nullptr) 
        : QStyledItemDelegate(parent) {}

    bool bold = false;
    QStringList mKeys;

    virtual void setKeyWords(const QStringList keys)
    {
        mKeys = keys;
    }

    class Builder {
        public:
            HighlightDelegate* mDelegate = nullptr;
            QObject* mParent = nullptr;
            Builder(QObject* parent = nullptr) : mParent(parent) {}
            virtual Builder& setBold(const bool value) = 0;
            virtual HighlightDelegate* build() = 0;
        };
};
#endif // HIGLIGHTDELEGATE_H
