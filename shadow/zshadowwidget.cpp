#include "zshadowwidget.h"
#include "ui_zshadowwidget.h"
#include "zshadowengine.h"
#include <QPaintEvent>
#include <QPainter>

ZShadowWidget::ZShadowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZShadowWidget)
{
    ui->setupUi(this);
//    ui->widget_bg->setVisible(false);
}

ZShadowWidget::~ZShadowWidget()
{
    delete ui;
}

void ZShadowWidget::paintEvent(QPaintEvent *event)
{
//    {
//        QPainter painter(this);
//        QColor color = QColor(255,255,255);
//        QBrush brush(color);
//        painter.setBrush(brush);
//        painter.drawRect(event->rect());
//        {
//            int radius = 50;
//            int x = 10;
//            QRect rect(x,x,radius,radius);
//            QPoint center(x+radius,x+radius);
//            QRadialGradient radialGradient(center,radius,center);
//            QColor shadowColor(0,0,255,222);
//            radialGradient.setColorAt(0, shadowColor);
////            radialGradient.setColorAt(1 - shawPos, m_color);
//        //    radialGradient.setColorAt(1,QColor(m_color.red(),m_color.green(),m_color.blue(),0));
//        //    radialGradient.setColorAt(0.8,QColor(255,255,255,255));
//            radialGradient.setColorAt(1,QColor(255,255,255,0));
//            QBrush brush(radialGradient);
//            painter.setBrush(brush);
//            QPen pen(QColor("#ff0000"));
//            pen.setWidth(1);
//            painter.setPen(pen);
//            painter.drawRect(rect);
//        }

//        return;
//    }
    QPainter painter(this);
//
    QColor color = QColor(227,0,0,255);
//    QPen pen(color);
    QPen pen("#ff0000");
//    painter.setPen(pen);
    painter.setPen(Qt::NoPen);
    int margin = 17;
    QBrush brush(color);
    m_engine.setColor(color);
    painter.setBrush(brush);
    painter.drawRect(QRect(margin,margin,event->rect().width() - 2 * margin,event->rect().height() - 2 * margin));
    m_engine.setRadius(margin);
    m_engine.painterShadow(this);
}
