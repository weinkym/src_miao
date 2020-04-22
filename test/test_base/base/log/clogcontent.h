#ifndef CLOGCONTENT_H
#define CLOGCONTENT_H
#include <QLine>
#include <QLineF>
#include <QTime>
#include <QDateTime>
#include <QSize>
#include <QSizeF>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QString>
#include <QUuid>
#include <QVariant>
#include <string>
#include <QDebug>

template<typename T> QString LJDebugString(T &t)
{
    QString ret;
    QDebug(&ret).noquote()<<t;
    return ret;
}

template<typename T>
class CLJLogCreate
{
public:
    static QString createValue(const T &value)
    {
      return LJDebugString(value);
    };

private:

};

template<>
class CLJLogCreate<std::string>
{
public:
    static QString createValue(const std::string &value)
    {
        return QString("%1")
                .arg(value.c_str());
    };

private:
};


template<>
class CLJLogCreate<QTime>
{
public:
    static QString createValue(const QTime &value)
    {
        return QString("(%1")
                .arg(value.toString("hh:mm:ss:zzz"));
    };

private:
};

template<>
class CLJLogCreate<QDateTime>
{
public:
    static QString createValue(const QDateTime &value)
    {
        return QString("(%1")
                .arg(value.toString("yyyyMMdd hh:mm:ss:zzz"));
    };

private:
};

template<typename T>
QString createLogContent(const T& data)
{
    return CLJLogCreate<T>::createValue(data);
}


#endif // CLOGCONTENT_H
