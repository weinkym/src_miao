#ifndef _CSCREENRECTITEM_H_
#define _CSCREENRECTITEM_H_
#include <QGraphicsObject>
#include "cscreenshot_global.h"
#include "cscreenpublic.h"

class CSCREENSHOTSHARED_EXPORT CScreenRectItem : public QGraphicsObject
{
    Q_OBJECT

public:
    CScreenRectItem(const QRectF &rect,const QLine &painterLine,QGraphicsItem *parent = 0);
    ~CScreenRectItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void setPainterLine(const QLine &painterLine);
    void setLineWidth(int width);
    void setLineColor(const QColor &color);
    void setType(CScreenButtonType type);

private:
    QPixmap createBackgroupdPixmap();

private:
    QRectF m_rect;
    QLine m_painterLine;
    int m_lineWidth;
    QColor m_lineColor;
    CScreenButtonType m_type;
};

#endif // _CSCREENRECTITEM_H_

