#ifndef ZWMOVEWIDGET_H
#define ZWMOVEWIDGET_H
#include "zwgui_global.h"

#include <QWidget>

class ZWMoveWidgetHandler;

class ZWGUISHARED_EXPORT ZWMoveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZWMoveWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ZWMoveWidget();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    ZWMoveWidgetHandler *m_moveHandler;
};

#endif // ZWMOVEWIDGET_H
