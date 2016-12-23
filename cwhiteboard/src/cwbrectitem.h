#ifndef CWBRECTITEM_H
#define CWBRECTITEM_H
#include <QGraphicsObject>
#include <QPen>
#include "cwhiteboard_global.h"

class CWHITEBOARDSHARED_EXPORT CWBRectItem : public QGraphicsObject
{
public:
    enum Type{
        TYPE_RECT,
        TYPE_ELLIPSE,
    };
    CWBRectItem(Type type,QGraphicsItem *parent = Q_NULLPTR);
    void setPosition(const QPointF &startPoint, const QPointF &endPoint);
    void setPen(const QPen &pen);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

private:
    void adjustBoundingRect();

private:
    Type m_type;
    QRectF m_rect;
    QRectF m_boundingRect;
    QPointF m_startPoint;
    QPointF m_endPoint;
    QPen m_pen;
};

#endif // CWBRECTITEM_H
