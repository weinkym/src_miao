#ifndef CWBTEXTITEM_H
#define CWBTEXTITEM_H
#include <QGraphicsTextItem>

class CWBTextItem : public QGraphicsTextItem
{
public:
    CWBTextItem(QGraphicsItem *parent = Q_NULLPTR);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    int addMargin() const{return m_addMargin;};

protected:
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    const static int m_addMargin = 5;
};

#endif // CWBTEXTITEM_H
