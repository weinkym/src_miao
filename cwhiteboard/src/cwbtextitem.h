#ifndef CWBTEXTITEM_H
#define CWBTEXTITEM_H
#include <QGraphicsTextItem>

class CWBTextItem : public QGraphicsTextItem
{
public:
    CWBTextItem(QGraphicsItem *parent = Q_NULLPTR);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    int addMargin() const{return m_addMargin;};

protected:
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    const static int m_addMargin = 5;
};

#endif // CWBTEXTITEM_H
