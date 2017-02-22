#ifndef CWBLINEITEM_H
#define CWBLINEITEM_H
#include <QGraphicsLineItem>

class CWBLineItem : public QGraphicsLineItem
{
public:
    CWBLineItem(QGraphicsItem *parent = Q_NULLPTR);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
};

#endif // CWBLINEITEM_H
