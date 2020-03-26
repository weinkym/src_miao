#ifndef ZSHADOWENGINE_H
#define ZSHADOWENGINE_H

#include <QColor>
#include <QWidget>


class ZShadowEngine
{
public:
    ZShadowEngine();
    ~ZShadowEngine();

    void setColor(const QColor &color);
    void setRadius(int radius);
    void painterShadow(QWidget *widget);

private:
    QBrush getBrush(const QPoint &center,int radius);
    void setLineGradientPainter(QPainter *painter,const QPointF &startPoint,const QPointF &endPoint);

private:
    QColor m_color;
    int m_radius;
};

#endif // ZSHADOWENGINE_H
