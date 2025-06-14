#include "HighlightDelegate.hpp"
#include <QApplication>
#include "Logger.hpp"

void HighlightDelegate::setKeywords(const QString& keywords)
{
    QStringList list = keywords.split("|", Qt::SkipEmptyParts);
    for (QString& word : list) {
        word = word.trimmed();
    }
    m_keywords = list;
}
void HighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
           const QModelIndex& index) const {
    QString text = index.data().toString();

    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    painter->save();

    QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt);
    painter->setFont(opt.font);

    int x = textRect.left();
    int y = textRect.top() + (textRect.height() + painter->fontMetrics().ascent() - painter->fontMetrics().descent()) / 2;

    QString remaining = text;

    while (!remaining.isEmpty()) {
        int firstMatchPos = -1;
        int matchLength = 0;
        int matchedIndex = -1;

        for (int i = 0; i < m_keywords.size(); ++i) {
            const QString& kw = m_keywords[i];
            int idx = remaining.indexOf(kw, 0, Qt::CaseInsensitive);
            if (idx >= 0 && (firstMatchPos == -1 || idx < firstMatchPos)) {
                firstMatchPos = idx;
                matchLength = kw.length();
                matchedIndex = i;
            }
        }

        if (firstMatchPos == -1) {
            painter->setPen(opt.palette.text().color());
            painter->drawText(x, y, remaining);
            break;
        }

        // Draw text before match
        QString before = remaining.left(firstMatchPos);
        painter->setPen(opt.palette.text().color());
        painter->drawText(x, y, before);
        x += painter->fontMetrics().horizontalAdvance(before);

        // Draw matched word
        QString match = remaining.mid(firstMatchPos, matchLength);
        QColor bgColor = (matchedIndex >= 0 && matchedIndex < m_colors.size())
                         ? m_colors[matchedIndex]
                         : QColorConstants::Gray;

        int width = painter->fontMetrics().horizontalAdvance(match);
        painter->fillRect(QRect(x, textRect.top(), width, textRect.height()), bgColor);
        painter->setPen(Qt::black);
        painter->drawText(x, y, match);
        x += width;

        remaining = remaining.mid(firstMatchPos + matchLength);
    }

    painter->restore();
}
