#ifndef CDRAWITEM_H
#define CDRAWITEM_H

#include <QGraphicsItem>

class CDrawItem : public QGraphicsItem
{
public:
   CDrawItem(QGraphicsItem *parent = Q_NULLPTR);
    ~CDrawItem();
    void setRect(const QRectF &rect);
    virtual QRectF boundingRect() const ;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
    virtual QPainterPath shape() const;

private:
    QRectF m_rect;
};

#endif // CDRAWITEM_H
