#include "cdrawitem.h"
#include <QPainter>
#include "cwbrectitem.h"
#include "cwbpathitem.h"
#include <QGraphicsScene>

#include "cwberaseritem.h"
#include "cwbtextitem.h"
#include "cwblineitem.h"
#include "cwbpointitem.h"

CDrawItem::CDrawItem(CWB::DrawParam param, qreal zValue, QGraphicsItem *parent)
    :QObject(NULL)
    ,m_rectItem(NULL)
    ,m_pathItem(NULL)
    ,m_eraserItem(NULL)
    ,m_textItem(NULL)
    ,m_pointItem(NULL)
    ,m_lineItem(NULL)
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
    case CWB::DRAW_TYPE_ARROW:
        m_rectItem = new CWBRectItem(CWBRectItem::TYPE_ARROW,parent);
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
    case CWB::DRAW_TYPE_TEXT:
    {
        m_textItem = new CWBTextItem(parent);
        m_textItem->setDefaultTextColor(m_drawParam.lineColor);
        m_textItem->setPlainText("");
        QFont font;
        font.setPixelSize(16);
        m_textItem->setFont(font);
        break;
    }
    case CWB::DRAW_TYPE_LINE:
        m_lineItem = new CWBLineItem(parent);
        m_lineItem->setPen(pen);
        m_lineItem->setZValue(zValue);
        break;
    case CWB::DRAW_TYPE_POINT:
        m_pointItem = new CWBPointItem(parent);
        m_pointItem->setColor(m_drawParam.lineColor);
        m_pointItem->setZValue(zValue);
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
    QGraphicsItem *item = this->item();

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
    m_lineItem = NULL;
    m_pointItem = NULL;
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
    else if(m_rectItem)
    {
        m_rectItem->setPosition(startPoint,endPoint);
    }
    else if(m_textItem)
    {
        QRectF rect = m_textItem->boundingRect();
        m_textItem->setPos(startPoint - QPointF(rect.width() / 2,rect.height() / 2) + QPointF(m_textItem->addMargin(),m_textItem->addMargin()));
    }
    else if(m_lineItem)
    {
        m_lineItem->setLine(QLineF(startPoint,endPoint));
    }
    else if(m_pointItem)
    {
        m_pointItem->setPos(startPoint - QPointF(m_pointItem->boundingRect().width() / 2,m_pointItem->boundingRect().height() / 2));
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

void CDrawItem::setVisible(bool visible)
{
    QGraphicsItem *item = this->item();
    if(item)
    {
        item->setVisible(visible);
    }
}

bool CDrawItem::isVisible()
{
    QGraphicsItem *item = this->item();
    if(item)
    {
        return item->isVisible();
    }
    return false;
}

QGraphicsItem *CDrawItem::item()
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
    else if(m_textItem)
    {
        item = m_textItem;
    }
    else if(m_lineItem)
    {
        item = m_lineItem;
    }
    else if(m_pointItem)
    {
        item = m_pointItem;
    }
    return item;
}
