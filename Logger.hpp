#ifndef LOGGER_H
#define LOGGER_H

#include "QString"


class Logger
{
public:
    static void d(const QString &tag, const QString &msg);
    static void w(const QString &tag, const QString &msg);
    static void e(const QString &tag, const QString &msg);

    static void setTimeFrom(const QString &name, qint64 timeFrom);
    static void setTimeTo(const QString &name, qint64 timeTo);
    static bool VERBOSE;
private:
    static QMap<QString, QPair<qint64, qint64>> mTimeStampMap;
};
#endif // LOGGER_H
