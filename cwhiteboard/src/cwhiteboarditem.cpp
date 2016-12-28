#include "cwhiteboarditem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QUndoStack>
#include "cdrawitem.h"
#include "cwbaddcommand.h"
#include "cwbclearcommand.h"

CWhiteBoardItem::CWhiteBoardItem()
{
    m_isPressed = false;
    m_currentItem = NULL;
    m_drawParam.lineColor = QColor("00000ff");
    m_drawParam.type = CWB::DRAW_TYPE_PEN;
    m_drawParam.width = 3;
    m_undoStack = new QUndoStack;
}

CWhiteBoardItem::~CWhiteBoardItem()
{
    if(m_undoStack)
    {
        m_undoStack->deleteLater();
        m_undoStack = NULL;
    }
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
    QList<CDrawItem *> visibleItems;
    for(auto item : m_drawItems)
    {
        if(item->isVisible())
        {
            visibleItems.append(item);
        }
    }
    if(!visibleItems.isEmpty())
    {
        CWBClearCommand *cmd = new CWBClearCommand(visibleItems);
        m_undoStack->push(cmd);
    }
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

void CWhiteBoardItem::undo()
{
    m_isPressed = false;
    if(m_currentItem)
    {
        m_currentItem->clear();
        m_currentItem->deleteLater();
        m_currentItem = NULL;
        return;
    }
//    if(m_drawItems.isEmpty())
//    {
//        return;
//    }
//    CDrawItem *item = m_drawItems.last();
//    m_drawItems.removeLast();
//    item->clear();
//    item->deleteLater();
    m_undoStack->undo();
}

void CWhiteBoardItem::redo()
{
    m_undoStack->redo();
}

void CWhiteBoardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
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
            m_currentItem = new CDrawItem(m_drawParam,m_drawItems.count(),this);
            m_currentItem->setBrush(QBrush(this->pixmap()));
            QList<CDrawItem*> itemList;
            itemList.append(m_currentItem);
            CWBAddCommand *cmd = new CWBAddCommand(itemList);
            m_undoStack->push(cmd);
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
