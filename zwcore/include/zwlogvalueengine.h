#ifndef ZWLOGVALUEENGINE_H
#define ZWLOGVALUEENGINE_H

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
#include <QImage>
#include <QPixmap>
#include <QDebug>

template <typename T>
QString ZWDebugString(T &t)
{
    QString ret;
    QDebug(&ret).noquote() << t;
    return ret;
}

template <typename T>
class ZWLogCreate
{
public:
    static QString createValue(const T &value)
    {
        return LJDebugString(value);
    };

private:
};

template <>
class ZWLogCreate<QColor>
{
public:
    static QString createValue(const QColor &value)
    {
        return QString("(name=%1,r=%2,g=%3,b=%4")
            .arg(value.name())
            .arg(value.red())
            .arg(value.green())
            .arg(value.blue());
    };

private:
};

template <>
class ZWLogCreate<QTime>
{
public:
    static QString createValue(const QTime &value)
    {
        return QString("(%1")
            .arg(value.toString("hh:mm:ss:zzz"));
    };

private:
};

template <>
class ZWLogCreate<QDateTime>
{
public:
    static QString createValue(const QDateTime &value)
    {
        return QString("(%1")
            .arg(value.toString("yyyyMMdd hh:mm:ss:zzz"));
    };

private:
};

template <>
class ZWLogCreate<std::string>
{
public:
    static QString createValue(const std::string &value)
    {
        return QString("(%1")
            .arg(value.c_str());
    };

private:
};

template<typename T>
QString createLogContent(const T& data)
{
    return ZWLogCreate<T>::createValue(data);
}
#endif // ZWLOGVALUEENGINE_H
