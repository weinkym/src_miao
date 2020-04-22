#ifndef CCLICKWIDGET_H
#define CCLICKWIDGET_H

#include "cbasewidget.h"

class CClickWidget : public CBaseWidget
{
    Q_OBJECT
public:
    explicit CClickWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CClickWidget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void sigClicked(const QUuid &uuid);

private:
    bool m_isLeftPressed;
};

#endif // CCLICKWIDGET_H
