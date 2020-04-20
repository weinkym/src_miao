#ifndef CWIDGET_H
#define CWIDGET_H
#include <QWidget>

class CWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CWidget();

protected:
    //    void paintEvent(QPaintEvent *event);
};

#endif // CWIDGET_H
