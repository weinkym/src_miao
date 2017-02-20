#ifndef CDRAWITEM_H
#define CDRAWITEM_H

#include <QGraphicsItem>
#include <cwhiteboardpublic.h>
#include "cwhiteboard_global.h"

class CWBRectItem;
class CWBPathItem;
class CWBEraserItem;
class CWBTextItem;
class CWBLineItem;
class CWBPointItem;

class CWHITEBOARDSHARED_EXPORT CDrawItem : public QObject
{
public:
   CDrawItem(CWB::DrawParam param,qreal zValue,QGraphicsItem *parent);
    ~CDrawItem();
    void clear();
    void setPosition(const QPointF &startPoint, const QPointF &endPoint);
    void setBrush(const QBrush &brush);
    void setVisible(bool visible);
    bool isVisible();


private:
    QGraphicsItem *item();

private:
    friend class CWhiteBoardItem;
    CWBRectItem *m_rectItem;
    CWBPathItem *m_pathItem;
    CWBEraserItem *m_eraserItem;
    CWBTextItem *m_textItem;
    CWBPointItem *m_pointItem;
    CWBLineItem *m_lineItem;
    CWB::DrawParam m_drawParam;
    QPointF m_startPoint;
    QPointF m_endPoint;
};

#endif // CDRAWITEM_H
