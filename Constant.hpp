#ifndef CONSTANT_HPP
#define CONSTANT_HPP
#include <QString>
class Constant final
{
public:
    class LogSplit final {
        public:
            inline static const QString OR = "|";
            inline static const QString AND = "&";
    };

    class LogLevel final {
        public:
            inline static const QString V = "V";
            inline static const QString D = "D";
            inline static const QString I = "I";
            inline static const QString W = "W";
            inline static const QString E = "E";
    };

    class TableLog final {
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
    };

    class TableLogMark final {
        public:
            static const int COL_LINE = 0;
            static const int COL_TIME = 1;
            static const int COL_PID = 2;
            static const int COL_TAG = 3;
            static const int COL_MSG = 4;

            inline static const QString LINE = "Line";
            inline static const QString TIME = "Time";
            inline static const QString PID = "PID";
            inline static const QString TAG = "Tag";
            inline static const QString MSG = "Message";
    };
};
#endif
