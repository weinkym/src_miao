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

template<typename T>
class ZWLogCreate
{
public:
    static QString createValue(const T &value)
    {
      return QString("%1").arg(value);
    };

private:

};

template<>
class ZWLogCreate<QLine>
{
public:
    static QString createValue(const QLine &value)
    {
        return QString("(pos1=%1,%2,pos2=%3,%4)").arg(value.x1()).arg(value.y1()).arg(value.x2()).arg(value.y2());
    };

private:
};

template<>
class ZWLogCreate<QLineF>
{
public:
    static QString createValue(const QLineF &value)
    {
        return QString("(pos1=%1,%2,pos2=%3,%4)").arg(value.x1()).arg(value.y1()).arg(value.x2()).arg(value.y2());
    };

private:
};

template<>
class ZWLogCreate<QPoint>
{
public:
    static QString createValue(const QPoint &value)
    {
        return QString("(x=%1,y=%2)").arg(value.x()).arg(value.y());
    };

private:
};

template<>
class ZWLogCreate<QPointF>
{
public:
    static QString createValue(const QPointF &value)
    {
        return QString("(x=%1,y=%2)").arg(value.x()).arg(value.y());
    };

private:
};

template<>
class ZWLogCreate<QSize>
{
public:
    static QString createValue(const QSize &value)
    {
        return QString("(w=%1,h=%2)").arg(value.width()).arg(value.height());
    };

private:
};

template<>
class ZWLogCreate<QSizeF>
{
public:
    static QString createValue(const QSizeF &value)
    {
        return QString("(w=%1,h=%2)").arg(value.width()).arg(value.height());
    };

private:
};

template<>
class ZWLogCreate<QRect>
{
public:
    static QString createValue(const QRect &value)
    {
        return QString("(x=%1,y=%2,w=%3,h=%4)").arg(value.x()).arg(value.y()).arg(value.width()).arg(value.height());
    };

private:
};

template<>
class ZWLogCreate<QRectF>
{
public:
    static QString createValue(const QRectF &value)
    {
        return QString("(x=%1,y=%2,w=%3,h=%4)").arg(value.x()).arg(value.y()).arg(value.width()).arg(value.height());
    };

private:
};

template<>
class ZWLogCreate<QTime>
{
public:
    static QString createValue(const QTime &value)
    {
        return QString("(%1)").arg(value.toString("hh:mm:ss"));
    };

private:
};

template<>
class ZWLogCreate<QDateTime>
{
public:
    static QString createValue(const QDateTime &value)
    {
        return QString("(%1)").arg(value.toString("yyyy-MM-dd hh:mm:ss"));
    };

private:
};

template<>
class ZWLogCreate<bool>
{
public:
    static QString createValue(const bool &value)
    {
        return QString("(%1)").arg(value ? "T":"F");
    };

private:
};

template<>
class ZWLogCreate<QByteArray>
{
public:
    static QString createValue(const QByteArray &value)
    {
        return QString(value);
    };

private:
};

template<>
class ZWLogCreate<QImage>
{
public:
    static QString createValue(const QImage &value)
    {
        return QString("(w=%1,h=%2,f=%3").arg(value.width()).arg(value.format());
    };

private:
};

template<>
class ZWLogCreate<QPixmap>
{
public:
    static QString createValue(const QPixmap &value)
    {
        return QString("(w=%1,h=%2").arg(value.width());
    };

private:
};

template<typename T>
QString createLogContent(const T& data)
{
    return ZWLogCreate<T>::createValue(data);
}
#endif // ZWLOGVALUEENGINE_H
