#ifndef CUTILINTERFACE_H
#define CUTILINTERFACE_H

#include <QLineF>
#include <QRectF>

class CUtilInterface
{
public:
    CUtilInterface();
    static bool adjuestLine(QLineF &line,const QRectF& rect);

    static QList<QLineF> getRectLines(const QRectF &rect);

    static void adjuestEllipsePoints(const QRectF &randRect,QList<QPointF>& points);

    static QList<QPointF> getEllipsePoints(const QRectF &randRect,const QRectF& srcRect);
    static QList<QPointF> getEllipsePoints(const QRectF& srcRect);
};

#endif // CUTILINTERFACE_H
