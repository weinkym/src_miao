#include "cwblineitem.h"
#include <QPainter>

CWBLineItem::CWBLineItem(QGraphicsItem *parent)
    :QGraphicsLineItem(parent)
{

}

void CWBLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    return QGraphicsLineItem::paint(painter,option,widget);
}
