#ifndef ZSHADOWWIDGET_H
#define ZSHADOWWIDGET_H

#include <QWidget>
#include "zshadowengine.h"

namespace Ui {
class ZShadowWidget;
}

class ZShadowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZShadowWidget(QWidget *parent = 0);
    ~ZShadowWidget();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::ZShadowWidget *ui;
    ZShadowEngine m_engine;
};

#endif // ZSHADOWWIDGET_H
