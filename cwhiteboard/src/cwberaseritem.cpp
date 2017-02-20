#include "cwberaseritem.h"
#include <QBrush>
#include <QColor>
#include <qmath.h>
#include <QDebug>
#include <QPen>
#include <QPainter>

QPointF gStartPoint;
CWBEraserItem::CWBEraserItem(QGraphicsItem *parent)
    :QGraphicsPathItem(parent)
    ,m_isFirst(true)
    ,m_radius(13)
{
    QPen pen(Qt::NoPen);
    this->setPen(pen);
}

void CWBEraserItem::appendLine(const QPointF &startPoint, const QPointF &endPoint)
{
    if(m_isFirst)
    {
        m_isFirst = false;
        m_prePoint = startPoint;
        m_rectList.append(QRectF(startPoint.x() - m_radius,startPoint.y() - m_radius,2 * m_radius,2 * m_radius));
        return;
    }
    QPainterPath path = this->path();
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(m_prePoint.x() - m_radius,m_prePoint.y() - m_radius,2 * m_radius,2 * m_radius);
    path.addEllipse(endPoint.x() - m_radius,endPoint.y() - m_radius,2 * m_radius,2 * m_radius);
    m_rectList.append(QRectF(endPoint.x() - m_radius,endPoint.y() - m_radius,2 * m_radius,2 * m_radius));

    QLineF line(m_prePoint,endPoint);
    if(line.length() < 3)
    {
        return;
    }
    qreal angele = line.angle() * 3.1415926 / 180;

    qreal dx = -qSin(angele) * m_radius;
    qreal dy = qCos(angele) * m_radius;
    qDebug()<<"m_prePoint"<<m_prePoint<<"endPoint"<<endPoint;
    qDebug()<<QString("angele=%1,dx=%2,dy=%3").arg(angele).arg(dx).arg(dy);
    QPointF pos1(m_prePoint.x() + dx,m_prePoint.y() - dy);
    QPointF pos2(m_prePoint.x() - dx,m_prePoint.y() + dy);
    QPointF pos3(endPoint.x() - dx,endPoint.y() + dy);
    QPointF pos4(endPoint.x() + dx,endPoint.y() - dy);
    QPolygonF polygon;
    polygon.append(pos1);
    polygon.append(pos2);
    polygon.append(pos3);
    polygon.append(pos4);
    polygon.append(pos1);
    path.addPolygon(polygon);
    m_polygonfList.append(polygon);
    m_prePoint = endPoint;
    this->setPath(path);
}

void CWBEraserItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(this->brush());
    painter->setPen(this->pen());
    for(auto obj:m_rectList)
    {
        painter->drawEllipse(obj);
    }
    for(auto obj:m_polygonfList)
    {
        painter->drawPolygon(obj);
    }
}
