#include "HighlightKey.hpp"
#include <QApplication>
#include <QStyle>
#include "Logger.hpp"
void HighlightKey::setKeyWords(const QStringList keys)
{
    mKeys = keys;
    mKeyTotal.clear();
    if (!mKeys.empty())
    {
        mKeyTotal.append(mKeys);
    }
    if (!mKeyFind.isEmpty())
    {
        mKeyTotal.append(mKeyFind);
    }
}

void HighlightKey::setKeyFind(const QString key)
{
    mKeyFind = key;
    mKeyTotal.clear();
    if (!mKeys.empty())
    {
        mKeyTotal.append(mKeys);
    }
    if (!mKeyFind.isEmpty())
    {
        mKeyTotal.append(mKeyFind);
    }
}

void HighlightKey::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

    painter->setFont(opt.font);
    int x = textRect.left();
    int y = textRect.top() + (textRect.height() + painter->fontMetrics().ascent() - painter->fontMetrics().descent()) / 2;

    QString remaining = text;

    while (!remaining.isEmpty())
    {
        int firstMatchPos = -1;
        int matchLength = 0;
        int matchedIndex = -1;

        for (int i = 0; i < mKeyTotal.size(); ++i)
        {
            const QString &kw = mKeyTotal[i];
            int idx = remaining.indexOf(kw, 0, Qt::CaseInsensitive);
            if (idx >= 0 && (firstMatchPos == -1 || idx < firstMatchPos))
            {
                firstMatchPos = idx;
                matchLength = kw.length();
                matchedIndex = i;
            }
        }

        if (firstMatchPos == -1)
        {
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
        QColor keyColor = (matchedIndex >= 0 && matchedIndex < mColors.size())
                             ? mColors[matchedIndex].first
                             : QColorConstants::Gray;

        int width = painter->fontMetrics().horizontalAdvance(match);
        painter->setPen(keyColor);

        if (QString::compare(match, mKeyFind, Qt::CaseInsensitive) == 0)
        {
            Logger::d("TAG", "match == mKeyFind");
            painter->fillRect(QRect(x, textRect.top(), width, textRect.height()), Qt::yellow);
            painter->setPen(Qt::red);
        }
        painter->setFont(fontKeyWord);
        painter->drawText(x, y, match);
        painter->setFont(opt.font);

        x += width;

        remaining = remaining.mid(firstMatchPos + matchLength);
    }

    painter->restore();
}
