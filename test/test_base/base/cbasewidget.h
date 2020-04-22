#ifndef CBASEWIDGET_H
#define CBASEWIDGET_H

#include <QUuid>
#include <QWidget>

class CBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CBaseWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CBaseWidget();
    QUuid getUuid() const;

protected:
    //    void paintEvent(QPaintEvent *event);

private:
    QUuid m_uuid;
};

#endif // CBASEWIDGET_H
