#ifndef HIGLIGHTDELEGATE_H
#define HIGLIGHTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextLayout>

class HighlightDelegate : public QStyledItemDelegate {
public:
    HighlightDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    void setKeywords(const QString& keywords);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
private:
    QStringList m_keywords;
};
#endif // HIGLIGHTDELEGATE_H
