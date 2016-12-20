#include "cdrawitem.h"
#include <QPainter>

CDrawItem::CDrawItem(QGraphicsItem *parent)
    :QGraphicsItem(parent)
    ,m_rect(-10,-10,100,100)
{

}

CDrawItem::~CDrawItem()
{

}


void CDrawItem::setRect(const QRectF &rect)
{
    m_rect = rect;
    update();
}

QRectF CDrawItem::boundingRect() const
{
    return m_rect;
}

void CDrawItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen pen(QColor("#00ff00"));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawRect(m_rect);
    painter->restore();
}

QPainterPath CDrawItem::shape() const
{
    QPainterPath path;
    path.addRect(0,0,10,10);
    return path;
}
