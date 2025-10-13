#ifndef UTILHELPER_HPP
#define UTILHELPER_HPP
#include <QString>
#include <QStringList>
#include "Constant.hpp"
#include <QTableWidgetItem>

class UtilHelper
{
public:
    static QStringList splitKeywords(const QString key)
    {
        if (key.contains(Constant::Split::AND))
        {
            return UtilHelper::splitKeywordsByAnd(key);
        }
        return UtilHelper::splitKeywordsByOr(key);
    }

    static QStringList splitKeywordsByOr(const QString key)
    {
        return key.split(Constant::Split::OR, Qt::SkipEmptyParts);
    }

    static QStringList splitKeywordsByAnd(const QString key)
    {
        return key.split(Constant::Split::AND, Qt::SkipEmptyParts);
    }

    static QTableWidgetItem* createTableItem(QString value, int direct)
    {
        QTableWidgetItem* item = new QTableWidgetItem(value);
        item->setTextAlignment(Qt::Alignment(direct));
        return item;
    }
};

#endif // UTILHELPER_HPP

