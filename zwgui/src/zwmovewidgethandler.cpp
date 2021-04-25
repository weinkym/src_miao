#include "zwmovewidgethandler.h"
#include <QWidget>
#include <QMouseEvent>

ZWMoveWidgetHandler::ZWMoveWidgetHandler(QWidget *w)
    :m_widget(w)
    ,m_isPressed(false)
{

}

ZWMoveWidgetHandler::~ZWMoveWidgetHandler()
{

}

void ZWMoveWidgetHandler::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = event->button() == Qt::LeftButton;
    if(m_isPressed)
    {
        m_pos = m_widget->pos();
        m_startPos = event->globalPos();
        m_endPos = m_startPos;
    }
}

void ZWMoveWidgetHandler::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
}

void ZWMoveWidgetHandler::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        m_endPos = event->globalPos();
        if(!m_widget.isNull())
        {
            m_widget->move(m_pos + m_endPos - m_startPos);
        }
    }
}
