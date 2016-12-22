#include "cdrawitem.h"
#include <QPainter>
#include "cwbrectitem.h"
#include "cwbpathitem.h"
#include <QGraphicsScene>

CDrawItem::CDrawItem(CWB::DrawParam param, QGraphicsItem *parent)
    :QObject(NULL)
    ,m_rectItem(NULL)
    ,m_pathItem(NULL)
    ,m_drawParam(param)
    ,m_startPoint(QPointF(0,0))
    ,m_endPoint(QPointF(0,0))
{
    QPen pen(m_drawParam.lineColor);
    pen.setWidth(m_drawParam.width);
    switch (m_drawParam.type)
    {
    case CWB::DRAW_TYPE_ELLIPSE:
        m_rectItem = new CWBRectItem(CWBRectItem::TYPE_ELLIPSE,parent);
        m_rectItem->setPen(pen);
        break;
    case CWB::DRAW_TYPE_PEN:
        m_pathItem = new CWBPathItem(parent);
        m_pathItem->setPen(pen);
        break;
    default:
        m_rectItem = new CWBRectItem(CWBRectItem::TYPE_RECT,parent);
        m_rectItem->setPen(pen);
        break;
    }
}

CDrawItem::~CDrawItem()
{
    clear();
}

void CDrawItem::clear()
{
    QGraphicsScene *scene = NULL;
    QGraphicsItem *item = NULL;
    if(m_rectItem)
    {
       item = m_rectItem;
    }
    else if(m_pathItem)
    {
        item = m_pathItem;
    }
    if(item)
    {
        QGraphicsScene *scene = item->scene();
        if(scene)
        {
            scene->removeItem(item);
        }
    }
    m_pathItem = NULL;
    m_rectItem = NULL;
}

void CDrawItem::setPosition(const QPointF &startPoint, const QPointF &endPoint)
{
    if(m_pathItem)
    {
        m_pathItem->appendLine(startPoint,endPoint);
    }
    else
    {
//        QRect rect(qMin(endPoint.x(),startPoint.x()),qMin(endPoint.y(),startPoint.y()),qAbs(endPoint.x() - startPoint.x()),qAbs(endPoint.y()-startPoint.y()));
//        m_rectItem->setRect(rect);
        m_rectItem->setPosition(startPoint,endPoint);
    }
}
