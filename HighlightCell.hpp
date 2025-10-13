#ifndef HighlightCell_H
#define HighlightCell_H
#include "HighlightDelegate.hpp"
#include <QApplication>
#include <QStyle>
class HighlightCell : public HighlightDelegate
{
private:
    QList<QPair<QColor, QColor>> mColors = {
        { QColorConstants::LightGray, QColorConstants::White },
        { QColorConstants::DarkBlue, QColorConstants::White },
        { QColorConstants::DarkGreen, QColorConstants::White },
        { QColorConstants::DarkRed, QColorConstants::White },
        { QColorConstants::Red, QColorConstants::White }
    };
public:
    HighlightCell(QObject* parent = nullptr) 
        : HighlightDelegate(parent) {}
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QString text = index.data().toString();

        QStyleOptionViewItem opt(option);
        initStyleOption(&opt, index);

        painter->save();

        QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

        QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt);
        QFont fontKeyWord = opt.font;
        fontKeyWord.setBold(bold);

        for (int i = 0; i < mKeys.size(); ++i) {
            if (text == mKeys[i]) {
                QColor bgColor = (i < mColors.size()) ? mColors[i].first : QColorConstants::Gray;
                painter->setFont(fontKeyWord);
                painter->fillRect(opt.rect, bgColor);
                painter->setPen(Qt::white);
                painter->drawText(textRect, opt.displayAlignment, text);
                painter->restore();
                return;
            }
        }
        painter->restore();
    }
    class Builder : public HighlightDelegate::Builder {
        public:
            Builder(QObject* parent = nullptr) : HighlightDelegate::Builder(parent)
            {
                delete mDelegate;
                mDelegate = new HighlightCell(parent);
            }
            Builder& setBold(const bool value) override
            {
                mDelegate->bold = value;
                return *this;
            }
            HighlightCell* build() override {
                return static_cast<HighlightCell*>(mDelegate);
        };
    };
};
#endif // HighlightCell_H
