#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "cscreeneditorbuttonitem.h"
#include <QDebug>

CScreenEditorButtonItem::CScreenEditorButtonItem(const CScreenEditorButtonItem::Param &param, QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_param(param)
    ,m_isHover(false)
    ,m_isPressed(false)
    ,m_normalBackground(QImage())
    ,m_hoverBackground(":/screenshot/res/screenshot/buttonhoverbg.png")
    ,m_pressedBackground(":/screenshot/res/screenshot/buttonpressedbg.png")
{

}

CScreenEditorButtonItem::~CScreenEditorButtonItem()
{

}

QRectF CScreenEditorButtonItem::boundingRect() const
{
    return m_param.rect;
}

void CScreenEditorButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    QPen pen(Qt::NoPen);
    painter->setPen(pen);
    if(m_isPressed)
    {
        painter->drawImage(m_param.rect,m_pressedBackground);
        painter->drawImage(m_param.rect,m_param.pressedImage);
    }
    else if(m_isHover)
    {
        painter->drawImage(m_param.rect,m_hoverBackground);
        painter->drawImage(m_param.rect,m_param.normalImage);
    }
    else
    {
        painter->drawImage(m_param.rect,m_normalBackground);
        painter->drawImage(m_param.rect,m_param.normalImage);
    }
    painter->restore();
}

void CScreenEditorButtonItem::setSelected(bool isSelected)
{
    if(m_isPressed != isSelected)
    {
        m_isPressed = isSelected;
        update();
    }
}

void CScreenEditorButtonItem::setIsHover(bool isHover)
{
    if(m_isHover != isHover)
    {
        m_isHover = isHover;
        update();
    }
}

void CScreenEditorButtonItem::setNormalBackground(const QImage &image)
{
    if(m_normalBackground == image)
    {
        return;
    }
    m_normalBackground = image;
    update();
}

void CScreenEditorButtonItem::setHoverBackground(const QImage &image)
{
    if(m_hoverBackground == image)
    {
        return;
    }
    m_hoverBackground = image;
    update();
}

void CScreenEditorButtonItem::setPressedBackground(const QImage &image)
{
    if(m_pressedBackground == image)
    {
        return;
    }
    m_pressedBackground = image;
    update();
}

void CScreenEditorButtonItem::setNormalIcon(const QImage &image)
{
    if(m_param.normalImage == image)
    {
        return;
    }
    m_param.normalImage = image;
    update();
}

void CScreenEditorButtonItem::setPressedIcon(const QImage &image)
{
    if(m_param.pressedImage == image)
    {
        return;
    }
    m_param.pressedImage = image;
    update();
}

void CScreenEditorButtonItem::setRect(const QRectF &rect)
{
    if(m_param.rect == rect)
    {
        return;
    }
    m_param.rect = rect;
    update();
}

void CScreenEditorButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        event->accept();
        update();
        emit sigClicked(m_param.type);
        return;
    }
    else
    {
        m_isPressed = false;
    }
    update();
    QGraphicsObject::mousePressEvent(event);
}

void CScreenEditorButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    C_SCREENSHOTSHARED_LOG(QString("mouseReleaseEvent"));
    QGraphicsObject::mouseReleaseEvent(event);
}

void CScreenEditorButtonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_isHover = true;
    update();
    QGraphicsObject::hoverEnterEvent(event);
}

void CScreenEditorButtonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_isHover = false;
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}

void CScreenEditorButtonItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    C_SCREENSHOTSHARED_LOG(QString("mouseDoubleClickEvent"));
    return QGraphicsObject::mouseDoubleClickEvent(event);
}
