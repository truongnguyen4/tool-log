#ifndef HighlightKey_H
#define HighlightKey_H
#include "HighlightDelegate.hpp"
#include <QApplication>
#include <QStyle>
#include "Logger.hpp"
class HighlightKey : public HighlightDelegate
{
private:
    QList<QPair<QColor, QColor>> mKeyFilterColors = {
        {QColor("#1976D2"), QColor("#E3F2FD")},   // Blue on light blue
        {QColor("#388E3C"), QColor("#C8E6C9")},   // Green on light green
        {QColor("#FBC02D"), QColor("#FFF9C4")},   // Yellow on light yellow
        {QColor("#D32F2F"), QColor("#FFCDD2")},   // Red on light red
        {QColor("#7B1FA2"), QColor("#E1BEE7")},   // Purple on light purple
        {QColor("#F57C00"), QColor("#FFE0B2")},   // Orange on light orange
        {QColor("#0288D1"), QColor("#B3E5FC")},   // Cyan on light cyan
        {QColor("#C2185B"), QColor("#F8BBD0")},   // Pink on light pink
        {QColor("#455A64"), QColor("#CFD8DC")},   // Blue Grey on light blue grey
        {QColor("#009688"), QColor("#B2DFDB")},   // Teal on light teal
        {QColor("#8D6E63"), QColor("#D7CCC8")}    // Brown on light brown
    };

    QList<QPair<QColor, QColor>> mKeyFindColors = {
        {QColor("#FF5722"), QColor("#FFE0B2")},   // Deep orange on pale orange
        {QColor("#3F51B5"), QColor("#C5CAE9")},   // Indigo on pale indigo
        {QColor("#009688"), QColor("#B2DFDB")},   // Teal on pale teal
        {QColor("#CDDC39"), QColor("#FFF9C4")},   // Lime on pale yellow
        {QColor("#E91E63"), QColor("#F8BBD0")},   // Pink on pale pink
        {QColor("#00BCD4"), QColor("#B3E5FC")},   // Cyan on pale cyan
        {QColor("#607D8B"), QColor("#CFD8DC")},   // Blue Grey on pale blue grey
        {QColor("#8BC34A"), QColor("#DCEDC8")},   // Light Green on pale green
        {QColor("#FFC107"), QColor("#FFECB3")},   // Amber on pale amber
        {QColor("#9C27B0"), QColor("#E1BEE7")},   // Purple on pale purple
        {QColor("#795548"), QColor("#D7CCC8")}    // Brown on pale brown
    };

    QStringList mKeyTotal;
    QStringList mKeyFind;
    void updateKeyTotal()
    {
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

public:
    HighlightKey(QObject *parent = nullptr) : HighlightDelegate(parent) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
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

            // Find the earliest keyword match
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
            if (!before.isEmpty())
            {
                painter->setPen(opt.palette.text().color());
                painter->drawText(x, y, before);
                x += painter->fontMetrics().horizontalAdvance(before);
            }

            // Draw matched word with highlight
            QString match = remaining.mid(firstMatchPos, matchLength);
            QColor textColor = QColorConstants::Gray;
            QColor bgColor = Qt::transparent;

            if (matchedIndex >= 0 && matchedIndex < mKeyFilterColors.size())
            {
                textColor = mKeyFilterColors[matchedIndex].first;
                // bgColor = mKeyFilterColors[matchedIndex].second;
            }

            // Check if match is in mKeyFind for special highlight
            int findIdx = mKeyFind.indexOf(match, 0, Qt::CaseInsensitive);
            if (findIdx != -1 && findIdx < mKeyFindColors.size())
            {
                textColor = mKeyFindColors[findIdx].first;
                bgColor = mKeyFindColors[findIdx].second;
            }

            int width = painter->fontMetrics().horizontalAdvance(match);
            QRect highlightRect(x, textRect.top(), width, textRect.height());

            if (bgColor != Qt::transparent)
            {
                painter->fillRect(highlightRect, bgColor);
            }

            painter->setFont(fontKeyWord);
            painter->setPen(textColor);
            painter->drawText(x, y, match);
            painter->setFont(opt.font);

            x += width;
            remaining = remaining.mid(firstMatchPos + matchLength);
        }

        painter->restore();
    }

    void setKeyWords(const QStringList keyWords) override
    {
        mKeys = keyWords;
        updateKeyTotal();
    }

    void setKeyFind(const QStringList keyFind)
    {
        mKeyFind = keyFind;
        updateKeyTotal();
    }

    class Builder : public HighlightDelegate::Builder
    {
    public:
        Builder(QObject *parent = nullptr) : HighlightDelegate::Builder(parent)
        {
            delete mDelegate;
            mDelegate = new HighlightKey(parent);
        }
        Builder &setBold(const bool value) override
        {
            mDelegate->bold = value;
            return *this;
        }
        HighlightKey *build() override
        {
            return static_cast<HighlightKey *>(mDelegate);
        }
    };
};
#endif // HighlightKey_H
