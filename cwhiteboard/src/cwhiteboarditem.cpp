#include "cwhiteboarditem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QUndoStack>
#include "cdrawitem.h"
#include "cwbaddcommand.h"
#include "cwbclearcommand.h"
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QGraphicsScene>
#include <QTimer>

CWhiteBoardItem::CWhiteBoardItem()
{
    m_isPressed = false;
    m_editable = true;
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
    m_currentItem = NULL;
    m_undoStack->undo();
}

void CWhiteBoardItem::redo()
{
    m_currentItem = NULL;
    m_undoStack->redo();
}

void CWhiteBoardItem::setEditable(bool enabled)
{
    if(m_editable == enabled)
    {
        return;
    }
    m_editable = enabled;
    if(!m_editable && m_currentItem)
    {
        QGraphicsItem *item = m_currentItem->item();
        if(item && item->isVisible())
        {
            m_currentItem->setVisible(false);
            m_currentItem->setVisible(true);
        }
    }
}

void CWhiteBoardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if(!m_editable)
    {
        return;
    }

    bool currentItemIsNull = (m_currentItem == NULL);
    if(m_currentItem)
    {
        m_currentItem = NULL;
    }
    if(m_drawParam.type == CWB::DRAW_TYPE_TEXT && m_isPressed && currentItemIsNull)
    {
        createCurrentItem();
        m_currentItem->setPosition(m_startPoint,m_startPoint);
        if(m_drawParam.type == CWB::DRAW_TYPE_TEXT)
        {
            QPointF scenePos = event->scenePos();
            event->accept();
            QTimer::singleShot(100, [this,scenePos]{
                if(this->scene())
                {
                    QGraphicsSceneMouseEvent pressedEvent(QEvent::GraphicsSceneMousePress);
                    pressedEvent.setScenePos(scenePos);
                    pressedEvent.setButton(Qt::LeftButton);
                    pressedEvent.setButtons(Qt::LeftButton);
                    QApplication::sendEvent(this->scene(),&pressedEvent);
                }
            });
        }
    }
    m_isPressed = false;
}

void CWhiteBoardItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_editable)
    {
        return;
    }
    if(m_isPressed && !isOnlyPoint())
    {
        QPointF endPoint = event->pos();
        createCurrentItem();
        m_currentItem->setPosition(m_startPoint,endPoint);
    }
}

void CWhiteBoardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_editable)
    {
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        m_startPoint = event->pos();

        if(isOnlyPoint())
        {
            if(m_drawParam.type == CWB::DRAW_TYPE_POINT)
            {
                createCurrentItem();
                m_currentItem->setPosition(m_startPoint,m_startPoint);
            }
        }
        else
        {
            m_currentItem = NULL;
        }
    }
    else
    {
        m_isPressed = false;
    }
}

void CWhiteBoardItem::createCurrentItem()
{
    if(m_currentItem == NULL)
    {
        m_currentItem = new CDrawItem(m_drawParam,m_drawItems.count(),this);
        m_currentItem->setBrush(QBrush(this->pixmap()));
        m_drawItems.append(m_currentItem);
        QList<CDrawItem*> itemList;
        itemList.append(m_currentItem);
        CWBAddCommand *cmd = new CWBAddCommand(itemList);
        m_undoStack->push(cmd);
    }
}

bool CWhiteBoardItem::isOnlyPoint()
{
    return (m_drawParam.type == CWB::DRAW_TYPE_TEXT || m_drawParam.type == CWB::DRAW_TYPE_POINT);
}
