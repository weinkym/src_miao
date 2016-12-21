#ifndef CDRAWITEM_H
#define CDRAWITEM_H

#include <QGraphicsItem>
#include <cwhiteboardpublic.h>

class CWBRectItem;
class CWBPathItem;

class CDrawItem : public QObject
{
public:
   CDrawItem(CWB::DrawParam param,QGraphicsItem *parent);
    ~CDrawItem();
    void setPosition(const QPointF &startPoint, const QPointF &endPoint);

private:
    CWBRectItem *m_rectItem;
    CWBPathItem *m_pathItem;
    CWB::DrawParam m_drawParam;
    QPointF m_startPoint;
    QPointF m_endPoint;
};

#endif // CDRAWITEM_H
