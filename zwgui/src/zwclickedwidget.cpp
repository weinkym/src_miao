#include "zwclickedwidget.h"

#include <QMouseEvent>

ZWClickedWidget::ZWClickedWidget(QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent,f)
    ,m_id(0)
    ,m_uuid(QUuid::createUuid())
{

}

ZWClickedWidget::~ZWClickedWidget()
{
    //
}

int ZWClickedWidget::getId() const
{
    return  m_id;
}

QUuid ZWClickedWidget::getUuid() const
{
    return m_uuid;
}

void ZWClickedWidget::setId(int id)
{
    m_id = id;
}

QVariant ZWClickedWidget::getUserData() const
{
    return m_userData;
}

void ZWClickedWidget::setUserData(const QVariant &value)
{
    m_userData = value;
}

void ZWClickedWidget::mousePressEvent(QMouseEvent *event)
{
    m_isLeftPressed = event->button() == Qt::LeftButton;
}

void ZWClickedWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_isLeftPressed && event->button())
    {
        m_isLeftPressed = false;
        emit sigClicked(m_id,m_uuid,m_userData);
    }
}
