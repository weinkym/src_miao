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
    QPainter painter(this);
//    painter.setPen(Qt::NoPen);
    QColor color("#cccccc");
//    QPen pen(color);
    QPen pen("#ff0000");
    painter.setPen(pen);
    int margin = 20;
    QBrush brush(color);
    m_engine.setColor(color);
    painter.setBrush(brush);
    painter.drawRect(QRect(margin,margin,event->rect().width() - 2 * margin,event->rect().height() - 2 * margin));
    m_engine.painterShadow(this);
}
