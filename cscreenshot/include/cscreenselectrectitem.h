#ifndef _CSCREENSELECTRECTITEM_H_
#define _CSCREENSELECTRECTITEM_H_

#include <QGraphicsRectItem>
#include "cscreenshot_global.h"
#include "cscreenpublic.h"

class CSCREENSHOTSHARED_EXPORT CScreenSelectRectItem : public QGraphicsRectItem
{
public:
    CScreenSelectRectItem(const QPixmap &desktopPixmap,QGraphicsItem * parent = 0);
    void setSelectedRect(const QRectF &rect);
    void setScale(qreal scale);
    ~CScreenSelectRectItem();
    QRectF getSelectRect() const;
    void setMovePointHidden(bool hidden);
    CScreenPositionType getPostionType(const QPointF &pos);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //TODO ==START== 为后期选择移动准备
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //TODO ==END== 为后期选择移动准备

private:
    //TODO ==START== 为后期选择移动准备
    QGraphicsEllipseItem *createEllipseItem(const QRectF &rect);
    void updateEllipseItems();
    void updateEllipseItem(QGraphicsEllipseItem **item,const QRectF &rect);
    //TODO ==END== 为后期选择移动准备

private:
    //TODO ==START== 为后期选择移动准备
    QGraphicsEllipseItem *m_topLeftEllipseItem;
    QGraphicsEllipseItem *m_bottomLeftEllipseItem;
    QGraphicsEllipseItem *m_topMiddleEllipseItem;
    QGraphicsEllipseItem *m_bottomMiddleEllipseItem;
    QGraphicsEllipseItem *m_topRightEllipseItem;
    QGraphicsEllipseItem *m_bottomRightEllipseItem;
    QGraphicsEllipseItem *m_leftMiddleEllipseItem;
    QGraphicsEllipseItem *m_rightMiddleEllipseItem;
    bool m_movePointHidden;
    //TODO ==END== 为后期选择移动准备
    QPixmap m_desktopPixmap;
    QColor m_penColor;
    QRectF m_rect;
    static const int m_ellipseRadius = 5;
    static const int m_ellipseLineWidth = 1;
    static const int m_ellipseOffset = 3;
    static const int m_penWidth = 4;// 边框宽度，尽量采用偶数
};

#endif // _CSCREENSELECTRECTITEM_H_

