#include <QPainter>
#include <QPixmap>
#include <QGraphicsScene>
#include "cscreenrectitem.h"
#include "cscreenshotutil.h"

CScreenRectItem::CScreenRectItem(const QRectF &rect, const QLine &painterLine, QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_rect(rect)
    ,m_painterLine(painterLine)
    ,m_lineWidth(1)
    ,m_type(CSCREEN_BUTTON_TYPE_UNDEFINED)
{

}

CScreenRectItem::~CScreenRectItem()
{

}

QRectF CScreenRectItem::boundingRect() const
{
    return m_rect;
}

void CScreenRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    QImage image = createBackgroupdPixmap().toImage();
    QRectF rect = boundingRect();
    painter->drawImage(rect.left(),rect.top(),image);
    painter->restore();
}

void CScreenRectItem::setPainterLine(const QLine &painterLine)
{
    prepareGeometryChange();
    m_painterLine = painterLine;
    update();
}

void CScreenRectItem::setLineWidth(int width)
{
    if(m_lineWidth == width)
    {
        return;
    }
    m_lineWidth = width;
    update();
}

void CScreenRectItem::setLineColor(const QColor &color)
{
    if(m_lineColor == color)
    {
        return;
    }
    m_lineColor = color;
    update();
}

void CScreenRectItem::setType(CScreenButtonType type)
{
    if(m_type == type)
    {
        return;
    }
    prepareGeometryChange();
    m_type = type;
    update();
}

QPixmap CScreenRectItem::createBackgroupdPixmap()
{
    QRectF rect = boundingRect();
    QRect paintRect = CScreenshotUtil::convertLineToRect(m_painterLine);
    qreal x = paintRect.x() - rect.x();
    qreal y = paintRect.y() - rect.y();
    QPixmap pixmap(rect.size().toSize());
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing);

//    painter.setRenderHints(QPainter::Antialiasing
//                            | QPainter::TextAntialiasing
//                            | QPainter::SmoothPixmapTransform
//                            | QPainter::HighQualityAntialiasing
//                            | QPainter::NonCosmeticDefaultPen
//                            | QPainter::Qt4CompatiblePainting);

    QPen pen;
    pen.setColor(m_lineColor);
    pen.setWidth(m_lineWidth);
    painter.setPen(pen);
//    painter.drawRect(QRect(x,y,m_painterRect.width(),m_painterRect.height()));
    painter.drawEllipse(QRect(x,y,paintRect.width(),paintRect.height()));
    return pixmap;
}
