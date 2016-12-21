#ifndef CWHITEBOARDITEM_H
#define CWHITEBOARDITEM_H

#include <QGraphicsPixmapItem>
#include "cwhiteboardpublic.h"

class CDrawItem;

class CWhiteBoardItem : public QGraphicsPixmapItem
{
public:
    enum State{
        STATE_SELECT,
        STATE_DRAW
    };

    CWhiteBoardItem();
    ~CWhiteBoardItem();
    void setState(CWhiteBoardItem::State state);
    void setLineColor(const QColor &color);
    void setLineWidth(int width);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    State m_state;
    bool m_isPressed;
    QPointF m_startPoint;
    CWB::DrawParam m_drawParam;
    CDrawItem *m_currentItem;
    QList<CDrawItem *> m_drawItem;
};

#endif // CWHITEBOARDITEM_H
