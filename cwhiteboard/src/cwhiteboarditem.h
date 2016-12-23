#ifndef CWHITEBOARDITEM_H
#define CWHITEBOARDITEM_H

#include <QGraphicsPixmapItem>
#include "cwhiteboardpublic.h"
#include "cwhiteboard_global.h"

class CDrawItem;

class CWHITEBOARDSHARED_EXPORT CWhiteBoardItem : public QGraphicsPixmapItem
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
    void clear();
    void setDrawType(CWB::DrawType type);
    void setDrawParam(CWB::DrawParam param);
    CWB::DrawParam getDrawParam() const{return m_drawParam;};

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
    QList<CDrawItem *> m_drawItems;
};

#endif // CWHITEBOARDITEM_H
