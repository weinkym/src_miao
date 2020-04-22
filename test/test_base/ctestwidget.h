#ifndef CTESTWIDGET_H
#define CTESTWIDGET_H
#include "cbasewidget.h"

class QSvgRenderer;
class CTestWidget : public CBaseWidget
{
    Q_OBJECT
public:
    explicit CTestWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CTestWidget();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onRepaintNeeded();

private:
    QSvgRenderer *m_svgRenderer;
};

#endif // CTESTWIDGET_H
