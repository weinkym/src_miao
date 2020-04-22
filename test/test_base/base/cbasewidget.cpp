#include "cbasewidget.h"
#include <QPainter>
#include <QStyleOption>
#include <QUuid>

CBaseWidget::CBaseWidget(CBaseWidget::QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_uuid(QUuid::createUuid())
{
    this->setAttribute(Qt::WA_StyledBackground, true);
}

CBaseWidget::~CBaseWidget()
{
}

QUuid CBaseWidget::getUuid() const
{
    return m_uuid;
}

//void CBaseWidget::paintEvent(QPaintEvent *e)
//{
//    Q_UNUSED(e);
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}
