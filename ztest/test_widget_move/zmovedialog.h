#ifndef ZMOVEDIALOG_H
#define ZMOVEDIALOG_H
#include <QDialog>

class ZMoveDialog : public QDialog
{
    Q_OBJECT

public:
    ZMoveDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ZMoveDialog();
    void setMoveWidget(QWidget *moveWidget);

protected:
    void timerEvent(QTimerEvent *event);

private:
    QWidget *m_moveWidget;
};

#endif // ZMOVEDIALOG_H
