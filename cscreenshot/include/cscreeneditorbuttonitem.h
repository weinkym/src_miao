#ifndef _CSCREENEDITORBUTTONITEM_H_
#define _CSCREENEDITORBUTTONITEM_H_
#include <QGraphicsObject>
#include "cscreenshot_global.h"
#include "cscreenpublic.h"

class CSCREENSHOTSHARED_EXPORT CScreenEditorButtonItem : public QGraphicsObject
{
    Q_OBJECT

public:
    struct Param
    {
        QRectF rect;
        CScreenButtonType type;
        QImage normalImage;
        QImage pressedImage;
        Param():rect(QRectF(0,0,1,1)),type(CSCREEN_BUTTON_TYPE_UNDEFINED){};
    };

    CScreenEditorButtonItem(const Param &param,QGraphicsItem *parent = 0);
    ~CScreenEditorButtonItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void setSelected(bool isSelected = false);
    void setIsHover(bool isHover);
    void setNormalBackground(const QImage &image);
    void setHoverBackground(const QImage &image);
    void setPressedBackground(const QImage &image);
    void setNormalIcon(const QImage &image);
    void setPressedIcon(const QImage &image);
    void setRect(const QRectF &rect);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void sigClicked(CScreenButtonType);

private:
    Param m_param;
    bool m_isHover;
    bool m_isPressed;
//    QImage m_imageNormal;
//    QImage m_imagePressed;
    QImage m_normalBackground;
    QImage m_hoverBackground;
    QImage m_pressedBackground;
//    QRectF m_rect;
};

#endif // _CSCREENEDITORBUTTONITEM_H_

