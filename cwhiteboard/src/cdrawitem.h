#ifndef CDRAWITEM_H
#define CDRAWITEM_H

#include <QGraphicsItem>
#include <cwhiteboardpublic.h>
#include "cwhiteboard_global.h"

class CWBRectItem;
class CWBPathItem;
class CWBEraserItem;

class CWHITEBOARDSHARED_EXPORT CDrawItem : public QObject
{
public:
   CDrawItem(CWB::DrawParam param,qreal zValue,QGraphicsItem *parent);
    ~CDrawItem();
    void clear();
    void setPosition(const QPointF &startPoint, const QPointF &endPoint);
    void setBrush(const QBrush &brush);

private:
    CWBRectItem *m_rectItem;
    CWBPathItem *m_pathItem;
    CWBEraserItem *m_eraserItem;
    CWB::DrawParam m_drawParam;
    QPointF m_startPoint;
    QPointF m_endPoint;
};

#endif // CDRAWITEM_H
