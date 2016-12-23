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
    m_rect = QRectF(qMin(m_endPoint.x(),m_startPoint.x()),qMin(m_endPoint.y(),m_startPoint.y()),qAbs(m_endPoint.x() - m_startPoint.x()),qAbs(m_endPoint.y()-m_startPoint.y()));
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
