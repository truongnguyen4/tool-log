#include "HighlightCell.hpp"
#include <QApplication>
#include <QStyle>

void HighlightCell::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
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
