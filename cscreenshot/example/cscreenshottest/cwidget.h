#ifndef CWIDGET_H
#define CWIDGET_H

#include <QWidget>

namespace Ui {
class CWidget;
}

class QProcess;

class CWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CWidget(QWidget *parent = 0);
    ~CWidget();

private slots:
    void on_btnStart_clicked();
    void readFromClient();

private:
    Ui::CWidget *ui;
    QProcess *m_process;
};

#endif // CWIDGET_H
