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

void HighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QString text = index.data().toString();

    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    painter->save();

    // Draw the background and selection
    QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt);
    painter->setFont(opt.font);

    // Draw highlighted text
    QString remaining = text;
    int x = textRect.left();
    int y = textRect.top() + (textRect.height() + painter->fontMetrics().ascent() - painter->fontMetrics().descent()) / 2;

    while (!remaining.isEmpty()) {
        int matchStart = -1;
        int matchLength = 0;
        QString matchWord;

        // Find first keyword match
        for (const QString& kw : m_keywords) {
            int idx = remaining.indexOf(kw, 0, Qt::CaseInsensitive);
            if (idx >= 0 && (matchStart == -1 || idx < matchStart)) {
                matchStart = idx;
                matchLength = kw.length();
                matchWord = kw;
            }
        }

        if (matchStart == -1) {
            // No match, draw the rest
            painter->setPen(opt.palette.text().color());
            painter->drawText(x, y, remaining);
            break;
        }

        // Draw before match
        QString before = remaining.left(matchStart);
        painter->setPen(opt.palette.text().color());
        painter->drawText(x, y, before);
        x += painter->fontMetrics().horizontalAdvance(before);

        // Draw match with background
        QString match = remaining.mid(matchStart, matchLength);
        int matchWidth = painter->fontMetrics().horizontalAdvance(match);

        painter->fillRect(QRect(x, textRect.top(), matchWidth, textRect.height()), QColor("#FFFF99"));
        painter->setPen(Qt::black);
        painter->drawText(x, y, match);
        x += matchWidth;

        // Move to remaining
        remaining = remaining.mid(matchStart + matchLength);
    }

    painter->restore();
}
