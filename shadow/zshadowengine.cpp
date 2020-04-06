#include "zshadowengine.h"
#include <QPainter>
#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QtMath>

ZShadowEngine::ZShadowEngine()
{
    m_color = QColor(227,227,227,128);
    m_radius = 20;
}

ZShadowEngine::~ZShadowEngine()
{

}

void ZShadowEngine::setColor(const QColor &color)
{
    m_color = color;
}

void ZShadowEngine::setRadius(int radius)
{
    if(radius >= 0)
    {
        m_radius = radius;
    }
}


void ZShadowEngine::painterShadow(QWidget *widget)
{
    if(m_radius <= 0)
    {
        return;
    }
    QPainter painter(widget);
    painterShadow(&painter,widget->rect());
}

void ZShadowEngine::painterShadow(QPainter *painter, const QRect &rect)
{
    if(!painter)
    {
        return;
    }
    int x = rect.x();
    int y = rect.y();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    int r = m_radius;
    painter->setPen(QPen(Qt::NoPen));
    {
        QRect rect(x,y,r,r);
        painter->setBrush(getBrush(rect.bottomRight(),r));
        painter->drawRect(rect);
    }
    {
        QRect rect(x,rect.height() - r + y,r,r);
        painter->setBrush(getBrush(rect.topRight(),r));
        painter->drawRect(rect);
    }
    {
        QRect rect(rect.width() - r+x,y,r,r);
        painter->setBrush(getBrush(rect.bottomLeft(),r));
        painter->drawRect(rect);
    }
    {
        QRect rect(rect.width() - r + x,rect.height() - r+y,r,r);
        painter->setBrush(getBrush(rect.topLeft(),r));
        painter->drawRect(rect);
    }
    qreal dd = 1.8;
    {
        setLineGradientPainter(painter,QPointF(r+x,y),QPointF(0-dd+x,y));
        painter->drawRect(QRect(x,m_radius+y,m_radius,rect.height() - 2 * r));
    }
    {
        setLineGradientPainter(painter,QPointF(rect.width() - r+x,0),QPointF(rect.width()+x,0));
        painter->drawRect(QRect(rect.width() - r+x,m_radius+y,m_radius,rect.height() - 2 * r));
    }
    {
        setLineGradientPainter(painter,QPointF(m_radius+x,m_radius+y),QPointF(m_radius+x,-dd+y));
        painter->drawRect(QRect(m_radius+x,y,rect.width() - 2 * m_radius,m_radius));
    }
    {
        setLineGradientPainter(painter,QPointF(m_radius+x,rect.height() - m_radius+y),QPointF(m_radius+x,rect.height()+y));
        painter->drawRect(QRect(m_radius+x,rect.height() - m_radius+y,rect.width() - 2 * m_radius,m_radius));
    }
}

void ZShadowEngine::painterShadow(QPainter *painter, const QRect &rect, int angle,
                                  int height, int position, const QColor &color)
{
    QPointF pos1;
    QPointF pos2;
    QPointF pos3;
    QPointF pos1D;
    QPointF pos2D;
    QPointF pos3D;
    qreal width = m_radius;
    int len_bottom_half = qTan(getAgnle(angle / 2)) * height;
    qreal len_2D2 = width / qSin(getAgnle(angle / 2));

    int flag = 1;
    switch (position) {
    case 1:
    {
        pos1 = QPointF(rect.center().x() - len_bottom_half,rect.y());
        pos2 = QPointF(rect.center().x(),rect.y() - height);
        pos3 = QPointF(rect.center().x() + len_bottom_half,rect.y());
        pos2D = QPointF(pos2.x(),pos2.y() - len_2D2);
        break;
    }
    case 2:
    {
        pos1 = QPointF(rect.center().x() + len_bottom_half,rect.bottom());
        pos2 = QPointF(rect.center().x(),rect.bottom() + height);
        pos3 = QPointF(rect.center().x() - len_bottom_half,rect.bottom());
        pos2D = QPointF(pos2.x(),pos2.y() + len_2D2);
        break;
    }
    case 3:
    {
        pos1 = QPointF(rect.left(),rect.center().y() - len_bottom_half);
        pos2 = QPointF(rect.left() - height,rect.center().y());
        pos3 = QPointF(rect.left(),rect.center().y() + len_bottom_half);
        pos2D = QPointF(pos2.x() - len_2D2,pos2.y());
        flag = -1;
        break;
    }
    case 4:
    {
        pos1 = QPointF(rect.right(),rect.center().y() + len_bottom_half);
        pos2 = QPointF(rect.right() + height,rect.center().y());
        pos3 = QPointF(rect.right(),rect.center().y() - len_bottom_half);
        pos2D = QPointF(pos2.x() + len_2D2,pos2.y());
        flag = -1;
        break;
    }
    default:
        break;
    }

    QLineF tLine(pos1,pos2);
    qreal tempAngle = (angle / 2 + 90) / 2;
    qreal len_1D1 = width / qSin(getAgnle(tempAngle));
    tLine.setAngle(tLine.angle() + flag * tempAngle);
    tLine.setLength(len_1D1);

    pos1D = tLine.p2();


    tempAngle = (angle / 2 + 90) / 2;
    tLine = QLineF(pos3,pos2);
    tLine.setAngle(tLine.angle() - flag * tempAngle);
    tLine.setLength(len_1D1);
    pos3D = tLine.p2();

    QPolygonF polygon1;
    polygon1.append(pos1);
    polygon1.append(pos2);
    polygon1.append(pos2D);
    polygon1.append(pos1D);
    polygon1.append(pos1);

    QPolygonF polygon2;
    polygon2.append(pos3);
    polygon2.append(pos2);
    polygon2.append(pos2D);
    polygon2.append(pos3D);
    polygon2.append(pos3);

    {
        painter->setPen(Qt::NoPen);
        tLine = QLineF(pos1D,pos2D);
        tLine.setAngle(tLine.angle() - 90);
        tLine.setLength(width * flag);
        QLinearGradient linearGradient(tLine.p2(),tLine.p1());
        linearGradient.setColorAt(0,m_color);
        linearGradient.setColorAt(1,QColor(255,255,255,0));
        painter->setBrush(QBrush(linearGradient));
        painter->drawPolygon(polygon1);
    }
    {
        painter->setPen(Qt::NoPen);
        tLine = QLineF(pos3D,pos2D);
        tLine.setAngle(tLine.angle() + 90);
        tLine.setLength(width * flag);
        QLinearGradient linearGradient(tLine.p2(),tLine.p1());
        linearGradient.setColorAt(0,m_color);
        linearGradient.setColorAt(1,QColor(255,255,255,0));
        painter->setBrush(QBrush(linearGradient));
        painter->drawPolygon(polygon2);
    }

   QRect shadowRect(rect.x() - width,rect.y()-width,rect.width() + width * 2,rect.height() + width * 2);
   painterShadow(painter,shadowRect);
   {
       painter->setPen(Qt::NoPen);
       painter->setBrush(QBrush(color));
       QPolygonF polygon1;
       polygon1.append(pos1);
       polygon1.append(pos2);
       polygon1.append(pos3);
       polygon1.append(pos1);
       painter->drawPolygon(polygon1);
       painter->drawRect(rect);
   }
}

QBrush ZShadowEngine::getBrush(const QPoint &center, int radius)
{
    QRadialGradient radialGradient(center,radius,center);
    radialGradient.setColorAt(0, m_color);
    radialGradient.setColorAt(1,QColor(255,255,255,0));
    return QBrush(radialGradient);
}

void ZShadowEngine::setLineGradientPainter(QPainter *painter, const QPointF &startPoint, const QPointF &endPoint)
{
    QLinearGradient linearGradient(startPoint,endPoint);
    linearGradient.setColorAt(0,m_color);
    linearGradient.setColorAt(1,QColor(255,255,255,0));
    painter->setBrush(QBrush(linearGradient));
}

qreal ZShadowEngine::getAgnle(qreal angle)
{
    return angle * 3.1415926 / 180;
}
