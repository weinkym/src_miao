#include "zwmovewidget.h"
#include "zwmovewidgethandler.h"

ZWMoveWidget::ZWMoveWidget(QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent,f)
    ,m_moveHandler(new ZWMoveWidgetHandler(this))
{

}

ZWMoveWidget::~ZWMoveWidget()
{
    if(m_moveHandler)
    {
        delete m_moveHandler;
        m_moveHandler = nullptr;
    }
}

void ZWMoveWidget::mousePressEvent(QMouseEvent *event)
{
    if(m_moveHandler)
    {
        m_moveHandler->mousePressEvent(event);
    }
}

void ZWMoveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_moveHandler)
    {
        m_moveHandler->mouseReleaseEvent(event);
    }
}

void ZWMoveWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moveHandler)
    {
        m_moveHandler->mouseMoveEvent(event);
    }
}
