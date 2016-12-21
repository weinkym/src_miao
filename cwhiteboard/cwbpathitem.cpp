#include "cwbpathitem.h"

CWBPathItem::CWBPathItem(QGraphicsItem *parent)
    :QGraphicsPathItem(parent)
    ,m_isFirst(true)
{

}

void CWBPathItem::appendLine(const QPointF &startPoint, const QPointF &endPoint)
{
    QPainterPath path =  this->path();
    if(m_isFirst)
    {
        m_isFirst = false;
        path.moveTo(startPoint);
    }
    path.lineTo(endPoint);
    path.moveTo(endPoint);
    this->setPath(path);
}
