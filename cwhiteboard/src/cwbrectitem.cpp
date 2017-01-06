#include "cwbrectitem.h"
#include <QPainter>
#include <QRectF>

CWBRectItem::CWBRectItem(Type type, QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_type(type)
{
    //    QGraphicsRectItem
}

void CWBRectItem::setPosition(const QPointF &startPoint, const QPointF &endPoint)
{
    m_startPoint = startPoint;
    m_endPoint = endPoint;
    QList<QPointF> pointList;
    pointList.append(m_startPoint);
    pointList.append(m_endPoint);
    if(m_type == TYPE_ARROW)
    {
        QLineF line(m_startPoint,m_endPoint);
        qreal length = line.length();
        if(length > m_arrowLength)
        {
            qreal dAngle = 45.0;
            QPointF arrowStartPoint = line.pointAt(1 - 1.0 * m_arrowLength / length);
            QLineF arrowLine = QLineF(m_endPoint,arrowStartPoint);
            QLineF arrowLineUp = arrowLine;
            arrowLineUp.setAngle(arrowLineUp.angle() + dAngle);
            QLineF arrowLineDown = arrowLine;
            arrowLineDown.setAngle(arrowLineDown.angle() - dAngle);

            m_arrowPointUp = arrowLineUp.p2();
            m_arrowPointDown = arrowLineDown.p2();;
            pointList.append(m_arrowPointUp);
            pointList.append(m_arrowPointDown);
        }
        else
        {
            m_arrowPointUp = endPoint;
            m_arrowPointUp = endPoint;
        }
    }
    m_rect = getPointsRect(pointList);
    adjustBoundingRect();
    prepareGeometryChange();
    update();
}

void CWBRectItem::setPen(const QPen &pen)
{
    m_pen = pen;
    adjustBoundingRect();
}

QRectF CWBRectItem::boundingRect() const
{
    return m_boundingRect;
}

void CWBRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->setPen(m_pen);
    if(m_type == TYPE_RECT)
    {
        painter->drawRect(m_rect);
    }
    else if(m_type == TYPE_ELLIPSE)
    {
        painter->drawEllipse(m_rect);
    }
    else if(m_type == TYPE_ARROW)
    {
        QLineF line(m_startPoint,m_endPoint);
        qreal length = line.length();
        if(length > m_arrowLength)
        {
            painter->drawLine(QLineF(m_endPoint,m_startPoint));
            painter->drawLine(QLineF(m_endPoint,m_arrowPointUp));
            painter->drawLine(QLineF(m_endPoint,m_arrowPointDown));
        }
    }
    painter->restore();
}

void CWBRectItem::adjustBoundingRect()
{
    qreal halfpw = m_pen.style() == Qt::NoPen ? qreal(0) : m_pen.widthF() / 2;
    m_boundingRect = m_rect;
    if(halfpw > 0.0)
    {
        m_boundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
}

QRectF CWBRectItem::getPointsRect(const QList<QPointF> &pointList)
{
    if(pointList.isEmpty())
    {
        return QRectF();
    }
    qreal minX = 0.0;
    qreal maxX = 0.0;
    qreal minY = 0.0;
    qreal maxY = 0.0;
    bool isFirst = true;
    for(auto obj : pointList)
    {
        if(isFirst)
        {
            isFirst = false;
            minX = obj.x();
            maxX = obj.x();
            minY = obj.y();
            maxY = obj.y();
        }
        else
        {
            minX = qMin(minX,obj.x());
            maxX = qMax(maxX,obj.x());
            minY = qMin(minY,obj.y());
            maxY = qMax(maxY,obj.y());
        }
    }
    return QRectF(minX,minY,maxX - minX,maxY - minY);
}
