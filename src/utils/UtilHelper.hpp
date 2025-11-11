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
        QString splitKey = Constant::Split::OR;
        if (key.contains(Constant::Split::AND))
        {
            splitKey = Constant::Split::AND;
        } else if (key.contains(Constant::Split::MINOR))
        {
            splitKey = Constant::Split::MINOR;
        }
        return key.split(splitKey, Qt::SkipEmptyParts);
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

    template <typename T>
    void updateHiddenByLine(T &object, const QString content, const QStringList keys)
    {
        if (keys.isEmpty())
        {
            return;
        }

        bool isHidden = true;
        if (content.toInt() >= keys[0].toInt() && content.toInt() <= keys[1].toInt())
        {
            isHidden = false;
        }
        object.setHidden(isHidden);
    }

    template <typename T>
    void updateHiddenByTime(T &object, const QString content, const QStringList keys)
    {
        if (keys.isEmpty())
        {
            return;
        }

        bool isHidden = true;
        if (toMilliseconds(content) >= toMilliseconds(keys[0]) && toMilliseconds(content) <= toMilliseconds(keys[1]))
        {
            isHidden = false;
        }
        object.setHidden(isHidden);
    }

    int toMilliseconds(const QString &time)
    {
        int h = time.mid(0, 2).toInt();
        int m = time.mid(3, 2).toInt();
        int s = time.mid(6, 2).toInt();
        int ms = time.mid(9, 3).toInt();
        return h * 3600000 + m * 60000 + s * 1000 + ms;
    }

    void updateFilter(QStringList &keys, bool &andOp, const QString key)
    {
        keys = UtilHelper::splitKeywords(key);
        andOp = key.contains(Constant::Split::AND);
    }

    void updateFilter(QStringList &keys, const QString key)
    {
        keys = UtilHelper::splitKeywords(key);
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
