#include "cwhiteboarditem.h"
#include <QGraphicsSceneMouseEvent>
#include "cdrawitem.h"

CWhiteBoardItem::CWhiteBoardItem()
{
    m_isPressed = false;
    m_currentItem = NULL;
    m_drawParam.lineColor = QColor("00000ff");
    m_drawParam.type = CWB::DRAW_TYPE_PEN;
    m_drawParam.width = 3;
}

CWhiteBoardItem::~CWhiteBoardItem()
{

}

void CWhiteBoardItem::setState(CWhiteBoardItem::State state)
{
    m_state = state;
}

void CWhiteBoardItem::setLineColor(const QColor &color)
{
    m_drawParam.lineColor = color;
}

void CWhiteBoardItem::setLineWidth(int width)
{
    if(width < 0)
    {
        return;
    }
    m_drawParam.width = width;
}

void CWhiteBoardItem::clear()
{
    for(auto item : m_drawItems)
    {
        item->clear();
        item->deleteLater();
    }
    m_drawItems.clear();
    m_currentItem = NULL;
}

void CWhiteBoardItem::setDrawType(CWB::DrawType type)
{
    m_drawParam.type = type;
}

void CWhiteBoardItem::setDrawParam(CWB::DrawParam param)
{
    m_drawParam = param;
}

void CWhiteBoardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_isPressed = false;
    if(m_currentItem)
    {
        m_drawItems.append(m_currentItem);
        m_currentItem = NULL;
    }
}

void CWhiteBoardItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isPressed)
    {
        QPointF endPoint = event->pos();
        if(m_currentItem == NULL)
        {
            m_currentItem = new CDrawItem(m_drawParam,this);
        }
        m_currentItem->setPosition(m_startPoint,endPoint);
    }
}

void CWhiteBoardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        m_startPoint = event->pos();
        m_currentItem = NULL;
    }
    else
    {
        m_isPressed = false;
    }
}
