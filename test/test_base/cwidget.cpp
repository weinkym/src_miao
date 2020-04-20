#include "cwidget.h"
#include <QPainter>
#include <QStyleOption>

CWidget::CWidget(CWidget::QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
}

CWidget::~CWidget()
{
}

//void CWidget::paintEvent(QPaintEvent *e)
//{
//    Q_UNUSED(e);
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}
