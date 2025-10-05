#ifndef HighlightCell_H
#define HighlightCell_H
#include "HighlightDelegate.hpp"
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
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
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
