#ifndef CWBPOINTITEM_H
#define CWBPOINTITEM_H
#include <QGraphicsObject>

class CWBPointItem : public QGraphicsObject
{
public:
    CWBPointItem(QGraphicsItem *parent = Q_NULLPTR);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
    void setColor(const QColor &color);

private:
    int m_radius;
    QColor m_color;
};

#endif // CWBPOINTITEM_H
