#ifndef HighlightKey_H
#define HighlightKey_H
#include "HighlightDelegate.hpp"
class HighlightKey : public HighlightDelegate
{
private:
    // Pair color Text - Background
    // TODO: Implement to use backgroud color
    QList<QPair<QColor, QColor>> mColors = {
        { QColorConstants::DarkGreen, QColorConstants::White },
        { QColorConstants::DarkRed, QColorConstants::White },
        { QColorConstants::Yellow, QColorConstants::White },
        { QColorConstants::Cyan, QColorConstants::White },
        { QColorConstants::DarkYellow, QColorConstants::White },
        { QColorConstants::DarkMagenta, QColorConstants::White },
        { QColorConstants::Gray, QColorConstants::White },
        { QColorConstants::Blue, QColorConstants::White },
        { QColorConstants::Green, QColorConstants::White },
        { QColorConstants::Magenta, QColorConstants::White },
        { QColorConstants::Red, QColorConstants::White }
    };
    QStringList mKeyTotal = QStringList();
    QString mKeyFind = "";
    void updateKeyTotal();
public:
    HighlightKey(QObject* parent = nullptr) : HighlightDelegate(parent) {}
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setKeyWords(const QStringList keyWords) override;
    void setKeyFind(const QString keyFind);

    class Builder : public HighlightDelegate::Builder {
        public:
        Builder(QObject* parent = nullptr) : HighlightDelegate::Builder(parent) 
        {
            delete mDelegate;
            mDelegate = new HighlightKey(parent);
        }
        Builder& setBold(const bool value) override
        {
            mDelegate->bold = value;
            return *this;
        }
        HighlightKey* build() override 
        {
            return static_cast<HighlightKey*>(mDelegate);
        }
    };

};
#endif // HighlightKey_H
