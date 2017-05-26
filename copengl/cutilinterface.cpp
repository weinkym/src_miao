#include "cutilinterface.h"
#include <QList>
#include <qmath.h>
#include <QDebug>

CUtilInterface::CUtilInterface()
{

}

bool CUtilInterface::adjuestLine(QLineF &line, const QRectF &rect)
{
   if(rect.contains(line.p1()) * rect.contains(line.p2()))
    {
        return true;
    }
   bool  isWapeX = (line.x1() < line.x2());
   QLineF orderLine = line;
   if(isWapeX)
   {
       orderLine = QLineF(line.p2(),line.p1());
   }
   QLineF top(rect.topLeft(),rect.topRight());
   QLineF right(rect.topRight(),rect.bottomRight());
   QLineF bottom(rect.bottomRight(),rect.bottomLeft());
   QLineF left(rect.bottomLeft(),rect.topLeft());
   QList<QLineF> rectLineList;
   rectLineList.append(top);
   rectLineList.append(right);
   rectLineList.append(bottom);
   rectLineList.append(left);
   QList<QPointF> intersectPointList;

   for(auto obj:rectLineList)
   {
       QPointF intersectPos;
       if(orderLine.intersect(obj,&intersectPos) == QLineF::BoundedIntersection)
       {
           intersectPointList.append(intersectPos);
       }
   }
   if(intersectPointList.count() == 1)
   {
       if(rect.contains(line.p1()))
        {
           intersectPointList.append(line.p1());
        }
       else
       {
           intersectPointList.append(line.p2());
       }
   }
   if(intersectPointList.count() == 2)
   {
       qreal d1 = QLineF(QPointF(0,0),line.p1() - intersectPointList.at(0)).length() + QLineF(QPointF(0,0),line.p1() - intersectPointList.at(1)).length();
       qreal d2 = QLineF(QPointF(0,0),line.p2() - intersectPointList.at(0)).length() + QLineF(QPointF(0,0),line.p2() - intersectPointList.at(1)).length();
       if(d1 < d2)
       {
           line.setP1(intersectPointList.at(0));
           line.setP2(intersectPointList.at(1));
       }
       else
       {
           line.setP1(intersectPointList.at(1));
           line.setP2(intersectPointList.at(0));
       }
       return true;
   }
   return false;
}

QList<QLineF> CUtilInterface::getRectLines(const QRectF &rect)
{
    QLineF top(rect.topLeft(),rect.topRight());
    QLineF right(rect.topRight(),rect.bottomRight());
    QLineF bottom(rect.bottomRight(),rect.bottomLeft());
    QLineF left(rect.bottomLeft(),rect.topLeft());
    QList<QLineF> rectLineList;
    rectLineList.append(top);
    rectLineList.append(right);
    rectLineList.append(bottom);
    rectLineList.append(left);
    return rectLineList;
}

void CUtilInterface::adjuestEllipsePoints(const QRectF &randRect, QList<QPointF> &points)
{
    QList<QLineF> rectLineList = getRectLines(randRect);
    for(int i = 0; i < points.count();  i++)
    {
        QLineF line(randRect.center(),points.at(i));
        for(auto lineObj:rectLineList)
        {
            QPointF intersectPos;
            if(line.intersect(lineObj,&intersectPos) == QLineF::BoundedIntersection)
            {
                qDebug()<<points.at(i) << intersectPos;
                points[i] = intersectPos;
                break;
            }
        }
    }
}

QList<QPointF> CUtilInterface::getEllipsePoints(const QRectF &randRect, const QRectF &srcRect)
{
    QList<QPointF> pointList = getEllipsePoints(srcRect);
    adjuestEllipsePoints(randRect,pointList);
    return pointList;
}

QList<QPointF> CUtilInterface::getEllipsePoints(const QRectF &srcRect)
{
    QList<QPointF> pointList;
    qDebug()<<"srcRect.center()"<<srcRect.center();
    int n = 360;
    for(int i = 0; i <= n; ++i)
    {
        pointList.append(srcRect.center() + QPointF(M_PI * qCos(2 * M_PI  / n * i) * srcRect.width() / 2, M_PI * qSin(2 * M_PI  /  n * i) * srcRect.height() / 2));
    }
    return pointList;
}
