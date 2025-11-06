#ifndef CONSTANT_HPP
#define CONSTANT_HPP
#include <QString>
#include <QList>
class Constant final
{
public:
    class TableSetting final
    {
    public:
        static const int COL_LINE = 0;
        static const int COL_GROUP = 1;
        static const int COL_SETTING = 2;
        static const int COL_VALUE = 3;
        static const int COL_SEND = 4;

        inline static const QString GROUP = "Group";
        inline static const QString SETTING = "Setting";
        inline static const QString VALUE = "Value";
        QList<int> getTableColumns() const
        {
            return QList<int>{COL_GROUP, COL_SETTING, COL_VALUE, COL_SEND};
        }
    };

    class TableProperty final
    {
    public:
        static const int COL_LINE = 0;
        static const int COL_PROPERTY = 1;
        static const int COL_VALUE = 2;
        static const int COL_SEND = 3;

        inline static const QString PROPERTY = "Property";
        inline static const QString VALUE = "value";
        QList<int> getTableColumns() const
        {
            return QList<int>{COL_PROPERTY, COL_VALUE, COL_SEND};
        }
    };

    class SettingGroup final
    {
    public:
        inline static const QString Global = "Global";
        inline static const QString System = "System";
        inline static const QString Secure = "Secure";
    };

    class Split final
    {
    public:
        inline static const QString OR = "|";
        inline static const QString AND = "&";
    };

    class LogLevel final
    {
    public:
        inline static const QString V = "V";
        inline static const QString D = "D";
        inline static const QString I = "I";
        inline static const QString W = "W";
        inline static const QString E = "E";
    };

    class TableLog final
    {
    public:
        static const int COL_LINE = 0;
        static const int COL_DATE = 1;
        static const int COL_TIME = 2;
        static const int COL_PID = 3;
        static const int COL_TID = 4;
        static const int COL_LEVEL = 5;
        static const int COL_TAG = 6;
        static const int COL_MSG = 7;

        inline static const QString LINE = "Line";
        inline static const QString DATE = "Date";
        inline static const QString TIME = "Time";
        inline static const QString PID = "PID";
        inline static const QString TID = "TID";
        inline static const QString LEVEL = "Level";
        inline static const QString TAG = "Tag";
        inline static const QString MSG = "Message";
        QList<int> getTableColumns() const
        {
            return QList<int>{COL_LINE, COL_DATE, COL_TIME, COL_PID, COL_TID, COL_LEVEL, COL_TAG, COL_MSG};
        }
    };

    class TableLogMark final
    {
    public:
        static const int COL_LINE = 0;
        static const int COL_DATE = 1;
        static const int COL_TIME = 2;
        static const int COL_PID = 3;
        static const int COL_TID = 4;
        static const int COL_LEVEL = 5;
        static const int COL_TAG = 6;
        static const int COL_MSG = 7;

        inline static const QString LINE = "Line";
        inline static const QString DATE = "Date";
        inline static const QString TIME = "Time";
        inline static const QString PID = "PID";
        inline static const QString TID = "TID";
        inline static const QString LEVEL = "Level";
        inline static const QString TAG = "Tag";
        inline static const QString MSG = "Message";
        QList<int> getTableColumns() const
        {
            return QList<int>{COL_LINE, COL_DATE, COL_TIME, COL_PID, COL_TID, COL_LEVEL, COL_TAG, COL_MSG};
        }
    };
};
#endif
