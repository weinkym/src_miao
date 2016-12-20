#ifndef CWHITEBOARDITEM_H
#define CWHITEBOARDITEM_H

#include <QGraphicsPixmapItem>

class CDrawItem;
class CWhiteBoardItem : public QGraphicsPixmapItem
{
public:
    CWhiteBoardItem();
    ~CWhiteBoardItem();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<CDrawItem *> m_drawItem;
};

#endif // CWHITEBOARDITEM_H
