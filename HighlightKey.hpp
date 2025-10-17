#ifndef HighlightKey_H
#define HighlightKey_H
#include "HighlightDelegate.hpp"
#include <QApplication>
#include <QStyle>

class HighlightKey : public HighlightDelegate
{
private:
    QList<QPair<QColor, QColor>> mKeyFilterColors = {
        {QColor(25, 118, 210), QColor(227, 242, 253)}, // Blue on light blue
        {QColor(56, 142, 60), QColor(200, 230, 201)},  // Green on light green
        {QColor(251, 192, 45), QColor(255, 249, 196)}, // Yellow on light yellow
        {QColor(211, 47, 47), QColor(255, 205, 210)},  // Red on light red
        {QColor(123, 31, 162), QColor(225, 190, 231)}, // Purple on light purple
        {QColor(245, 124, 0), QColor(255, 224, 178)},  // Orange on light orange
        {QColor(2, 136, 209), QColor(179, 229, 252)},  // Cyan on light cyan
        {QColor(194, 24, 91), QColor(248, 187, 208)},  // Pink on light pink
        {QColor(69, 90, 100), QColor(207, 216, 220)},  // Blue Grey on light blue grey
        {QColor(0, 150, 136), QColor(178, 223, 219)},  // Teal on light teal
        {QColor(141, 110, 99), QColor(215, 204, 200)}  // Brown on light brown
    };

    QList<QPair<QColor, QColor>> mKeyFindColors = {
        {QColor(255, 87, 34), QColor(255, 224, 178)},  // Deep orange on pale orange
        {QColor(63, 81, 181), QColor(197, 202, 233)},  // Indigo on pale indigo
        {QColor(0, 150, 136), QColor(178, 223, 219)},  // Teal on pale teal
        {QColor(205, 220, 57), QColor(255, 249, 196)}, // Lime on pale yellow
        {QColor(233, 30, 99), QColor(248, 187, 208)},  // Pink on pale pink
        {QColor(0, 188, 212), QColor(179, 229, 252)},  // Cyan on pale cyan
        {QColor(96, 125, 139), QColor(207, 216, 220)}, // Blue Grey on pale blue grey
        {QColor(139, 195, 74), QColor(220, 237, 200)}, // Light Green on pale green
        {QColor(255, 193, 7), QColor(255, 236, 179)},  // Amber on pale amber
        {QColor(156, 39, 176), QColor(225, 190, 231)}, // Purple on pale purple
        {QColor(121, 85, 72), QColor(215, 204, 200)}   // Brown on pale brown
    };

    QStringList mKeyTotal;
    QStringList mKeyFinds;
    void updateKeyTotal()
    {
        mKeyTotal.clear();
        if (!mKeys.empty())
        {
            mKeyTotal.append(mKeys);
        }
        if (!mKeyFinds.isEmpty())
        {
            mKeyTotal.append(mKeyFinds);
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
        // fontKeyWord.setBold(bold);

        painter->setFont(opt.font);
        QFontMetrics fm = painter->fontMetrics();
        int x0 = textRect.left();
        int y = textRect.top() + (textRect.height() + fm.ascent() - fm.descent()) / 2;

        // Draw the full base text first (so highlights can be painted on top)
        painter->setPen(opt.palette.text().color());
        painter->drawText(x0, y, text);

        // Helper match record
        struct Match
        {
            int start;
            int length;
            QColor textColor;
            QColor bgColor;
        };
        QList<Match> matches;

        // 1) Collect mKeys matches and paint them (first pass)
        for (int k = 0; k < mKeys.size(); ++k)
        {
            const QString &kw = mKeys.at(k);
            if (kw.isEmpty())
                continue;
            int pos = 0;
            while (true)
            {
                int idx = text.indexOf(kw, pos, Qt::CaseInsensitive);
                if (idx < 0)
                    break;
                Match m;
                m.start = idx;
                m.length = kw.length();

                int colorIndex = k % mKeyFilterColors.size();
                m.textColor = mKeyFilterColors[colorIndex].first;
                m.bgColor = Qt::transparent; // Current don't use background color

                matches.append(m);
                pos = idx + m.length;
            }
        }

        // Sort matches by start so drawing is left-to-right
        std::sort(matches.begin(), matches.end(), [](const Match &a, const Match &b)
                  { return a.start < b.start; });

        // Paint mKeys highlights (background + text)
        for (const Match &m : matches)
        {
            int x = x0 + fm.horizontalAdvance(text.left(m.start));
            int width = fm.horizontalAdvance(text.mid(m.start, m.length));
            QRect highlightRect(x, textRect.top(), width, textRect.height());
            if (m.bgColor.isValid() && m.bgColor != Qt::transparent)
                painter->fillRect(highlightRect, m.bgColor);
            painter->setPen(m.textColor.isValid() ? m.textColor : opt.palette.text().color());
            painter->setFont(fontKeyWord);
            painter->drawText(x, y, text.mid(m.start, m.length));
            painter->setFont(opt.font);
        }

        // 2) Collect mKeyFinds matches and paint them over (second pass)
        QList<Match> findMatches;
        for (int k = 0; k < mKeyFinds.size(); ++k)
        {
            const QString &kw = mKeyFinds.at(k);
            if (kw.isEmpty())
                continue;
            int pos = 0;
            while (true)
            {
                int idx = text.indexOf(kw, pos, Qt::CaseInsensitive);
                if (idx < 0)
                    break;
                Match m;
                m.start = idx;
                m.length = kw.length();
                if (!mKeyFindColors.isEmpty())
                {
                    int colorIndex = k % mKeyFindColors.size();
                    m.textColor = mKeyFindColors[colorIndex].first;
                    m.bgColor = mKeyFindColors[colorIndex].second;
                }
                else
                {
                    m.textColor = opt.palette.text().color();
                    m.bgColor = Qt::transparent;
                }
                findMatches.append(m);
                pos = idx + m.length;
            }
        }

        std::sort(findMatches.begin(), findMatches.end(), [](const Match &a, const Match &b)
                  { return a.start < b.start; });

        // Paint mKeyFinds highlights (overrides previous)
        for (const Match &m : findMatches)
        {
            int x = x0 + fm.horizontalAdvance(text.left(m.start));
            int width = fm.horizontalAdvance(text.mid(m.start, m.length));
            QRect highlightRect(x, textRect.top(), width, textRect.height());
            if (m.bgColor.isValid() && m.bgColor != Qt::transparent)
                painter->fillRect(highlightRect, m.bgColor);
            painter->setPen(m.textColor.isValid() ? m.textColor : opt.palette.text().color());
            painter->setFont(fontKeyWord);
            painter->drawText(x, y, text.mid(m.start, m.length));
            painter->setFont(opt.font);
        }

        painter->restore();
    }

    void setKeyWords(const QStringList keyWords) override
    {
        mKeys.clear();
        if (!keyWords.isEmpty())
        {
            mKeys.append(keyWords);
        }
        updateKeyTotal();
    }

    void setKeyFind(const QStringList keyWords)
    {
        mKeyFinds.clear();
        if (!keyWords.isEmpty())
        {
            mKeyFinds.append(keyWords);
        }
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
