#ifndef CONSTANT_HPP
#define CONSTANT_HPP
#include <string>
using std::string;
class Constant final
{
public:
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

            static constexpr const char* LINE = "Line";
            static constexpr const char* DATE = "Date";
            static constexpr const char* TIME = "Time";
            static constexpr const char* PID = "PID";
            static constexpr const char* TID = "TID";
            static constexpr const char* LEVEL = "Level";
            static constexpr const char* TAG = "Tag";
            static constexpr const char* MSG = "Message";
    };

    class TableLogMark final {
        public:
            static const int COL_LINE = 0;
            static const int COL_PID = 1;
            static const int COL_TAG = 2;
            static const int COL_MSG = 3;

            static constexpr const char* LINE = "Line";
            static constexpr const char* PID = "PID";
            static constexpr const char* TAG = "Tag";
            static constexpr const char* MSG = "Message";
    };
};
#endif
