#ifndef CWBERASERITEM_H
#define CWBERASERITEM_H
#include <QGraphicsPathItem>
#include "cwhiteboard_global.h"

class CWHITEBOARDSHARED_EXPORT CWBEraserItem : public QGraphicsPathItem
{
public:
    CWBEraserItem(QGraphicsItem *parent = Q_NULLPTR);
    void appendLine(const QPointF &startPoint, const QPointF &endPoint);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int m_radius;
    bool m_isFirst;
    QList<QPolygonF> m_polygonfList;
    QList<QRectF> m_rectList;
    QPointF m_prePoint;
};


#endif // CWBERASERITEM_H
