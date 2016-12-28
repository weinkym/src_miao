#include "cdrawitem.h"
#include <QPainter>
#include "cwbrectitem.h"
#include "cwbpathitem.h"
#include <QGraphicsScene>
#include "cwberaseritem.h"

CDrawItem::CDrawItem(CWB::DrawParam param, qreal zValue, QGraphicsItem *parent)
    :QObject(NULL)
    ,m_rectItem(NULL)
    ,m_pathItem(NULL)
    ,m_eraserItem(NULL)
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
        m_rectItem->setZValue(zValue);
        break;
    case CWB::DRAW_TYPE_PEN:
        m_pathItem = new CWBPathItem(parent);
        m_pathItem->setPen(pen);
        m_pathItem->setZValue(zValue);
        break;
    case CWB::DRAW_TYPE_ERASER:
        m_eraserItem = new CWBEraserItem(parent);
        m_eraserItem->setZValue(zValue);
    break;
    default:
        m_rectItem = new CWBRectItem(CWBRectItem::TYPE_RECT,parent);
        m_rectItem->setPen(pen);
        m_rectItem->setZValue(zValue);
        break;
    }
}

CDrawItem::~CDrawItem()
{
    clear();
}

void CDrawItem::clear()
{
    QGraphicsItem *item = NULL;
    if(m_rectItem)
    {
       item = m_rectItem;
    }
    else if(m_pathItem)
    {
        item = m_pathItem;
    }
    else if(m_eraserItem)
    {
        item = m_eraserItem;
    }
    if(item)
    {
        item->setVisible(false);
        QGraphicsScene *scene = item->scene();
        if(scene)
        {
            scene->removeItem(item);
        }
    }
    m_pathItem = NULL;
    m_rectItem = NULL;
    m_eraserItem = NULL;
}

void CDrawItem::setPosition(const QPointF &startPoint, const QPointF &endPoint)
{
    if(m_pathItem)
    {
        m_pathItem->appendLine(startPoint,endPoint);
    }
    else if(m_eraserItem)
    {
        m_eraserItem->appendLine(startPoint,endPoint);
    }
    else
    {
        m_rectItem->setPosition(startPoint,endPoint);
    }
}

void CDrawItem::setBrush(const QBrush &brush)
{
    if(m_eraserItem)
    {
       m_eraserItem->setBrush(brush);
    }
    else if(m_pathItem)
    {
        m_pathItem->setBrush(brush);
    }
}
