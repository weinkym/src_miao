#ifndef ZSHADOWENGINE_H
#define ZSHADOWENGINE_H

#include <QColor>
#include <QWidget>


class ZShadowEngine
{
public:
    ZShadowEngine();
    ~ZShadowEngine();
    static qreal getAgnle(qreal angle);

    void setColor(const QColor &color);
    void setRadius(int radius);
    void painterShadow(QWidget *widget);

    void painterShadow(QPainter *painter,const QRect &rect);

    void painterShadow(QPainter *painter,const QRect &rect,int angle,int height,int position,const QColor &color);

private:
    QBrush getBrush(const QPoint &center,int radius);
    void setLineGradientPainter(QPainter *painter,const QPointF &startPoint,const QPointF &endPoint);

private:
    QColor m_color;
    int m_radius;
};

#endif // ZSHADOWENGINE_H
