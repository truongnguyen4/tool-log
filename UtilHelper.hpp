#ifndef UTILHELPER_HPP
#define UTILHELPER_HPP
#include <QString>
#include <QStringList>
#include "Constant.hpp"
#include <QTableWidgetItem>

class UtilHelper
{
private:
    static inline UtilHelper *mInstance = nullptr;
    UtilHelper() {}

public:
    static UtilHelper *getInstance()
    {
        if (mInstance == nullptr)
        {
            mInstance = new UtilHelper();
        }
        return mInstance;
    }
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

    static QTableWidgetItem *createTableItem(QString value, int direct)
    {
        QTableWidgetItem *item = new QTableWidgetItem(value);
        item->setTextAlignment(Qt::Alignment(direct));
        return item;
    }

    template <typename T>
    void updateHidden(T &object, const QString content, const QStringList keys, const bool andOp)
    {
        if (keys.isEmpty())
        {
            return;
        }
        if (andOp)
        {
            bool isHidden = false;
            for (const QString &key : keys)
            {
                if (!content.contains(key, Qt::CaseInsensitive))
                {
                    isHidden = true;
                    break;
                }
            }
            object.setHidden(isHidden);
        }
        else
        {
            bool isHidden = true;
            for (const QString &key : keys)
            {
                if (content.contains(key, Qt::CaseInsensitive))
                {
                    isHidden = false;
                    break;
                }
            }
            object.setHidden(isHidden);
        }
    }

    void updateFilter(QStringList &keys, bool &andOp, const QString key)
    {
        keys = UtilHelper::splitKeywords(key);
        andOp = key.contains(Constant::Split::AND);
    }

    template <typename T>
    void clearListData(int &static_id, QList<T> &mListObjs)
    {
        static_id = 0;
        mListObjs.clear();
    }

    void addKey(QStringList &keys, int &id, const QString &key)
    {
        if (!key.isEmpty())
        {
            if (!keys.contains(key))
            {
                keys.push_back(key);
                id = keys.size() - 1;
            }
            else
            {
                // Swap exist key to the last
                keys.append(keys.takeAt(keys.indexOf(key)));
            }
        }
    }
};

#endif // UTILHELPER_HPP
