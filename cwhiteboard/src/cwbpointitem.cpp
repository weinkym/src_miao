#include "cwbpointitem.h"
#include <QPainter>

CWBPointItem::CWBPointItem(QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_radius(30)
{

}

QRectF CWBPointItem::boundingRect() const
{
    return QRectF(0,0,m_radius,m_radius);
}

void CWBPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::NoPen));
    painter->setBrush(QBrush(m_color));
    painter->drawEllipse(this->boundingRect());
}

void CWBPointItem::setColor(const QColor &color)
{
    m_color = color;
    m_color.setAlphaF(0.5);
    prepareGeometryChange();
    update();
}
