#include "cwbtextitem.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

CWBTextItem::CWBTextItem(QGraphicsItem *parent)
    :QGraphicsTextItem(parent)
{
    this->setTextInteractionFlags(Qt::TextEditorInteraction);
}

QRectF CWBTextItem::boundingRect() const
{
    QRectF rect = QGraphicsTextItem::boundingRect();
    rect.adjust(-m_addMargin,-m_addMargin, m_addMargin, m_addMargin);
    return rect;
}

QPainterPath CWBTextItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void CWBTextItem::focusOutEvent(QFocusEvent *event)
{
    if(this->textInteractionFlags() == Qt::TextEditorInteraction)
    {
        this->setTextInteractionFlags(Qt::NoTextInteraction);
    }
    this->setFlag(QGraphicsItem::ItemIsSelectable,false);
    if(!this->toPlainText().simplified().isEmpty())
    {
        //TODO 暂时屏蔽,目前还存在逻辑问题
//        this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    }
    QGraphicsTextItem::focusOutEvent(event);
}

void CWBTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    return QGraphicsTextItem::mouseReleaseEvent(event);
}
