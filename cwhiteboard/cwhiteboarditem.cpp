#include "cwhiteboarditem.h"
#include "cdrawitem.h"

CWhiteBoardItem::CWhiteBoardItem()
{
    CDrawItem* item = new CDrawItem(this);
    item->setFlag(QGraphicsItem::ItemClipsChildrenToShape,true);
}

CWhiteBoardItem::~CWhiteBoardItem()
{

}

void CWhiteBoardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //
}

void CWhiteBoardItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void CWhiteBoardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}
