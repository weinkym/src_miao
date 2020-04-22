#include "cclickwidget.h"
#include <QMouseEvent>

CClickWidget::CClickWidget(QWidget *parent, Qt::WindowFlags f)
    : CBaseWidget(parent, f)
    , m_isLeftPressed(false)
{
}

CClickWidget::~CClickWidget()
{
}

void CClickWidget::mousePressEvent(QMouseEvent *event)
{
    m_isLeftPressed = (event->button() == Qt::LeftButton);
    return CBaseWidget::mousePressEvent(event);
}

void CClickWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_isLeftPressed && event->button() == Qt::LeftButton)
    {
        emit sigClicked(this->getUuid());
    }
    m_isLeftPressed = false;
    return CBaseWidget::mouseReleaseEvent(event);
}
