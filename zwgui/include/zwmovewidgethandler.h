#ifndef ZWMOVEWIDGETHANDLER_H
#define ZWMOVEWIDGETHANDLER_H

#include <QPointer>
#include <QPoint>
#include "zwgui_global.h"

class QWidget;
class QMouseEvent;

class ZWGUISHARED_EXPORT ZWMoveWidgetHandler
{
public:
    ZWMoveWidgetHandler(QWidget *w);
    ~ZWMoveWidgetHandler();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPointer<QWidget> m_widget;
    bool m_isPressed;
    QPoint m_pos;
    QPoint m_startPos;
    QPoint m_endPos;
};

#endif // ZWMOVEWIDGETHANDLER_H
