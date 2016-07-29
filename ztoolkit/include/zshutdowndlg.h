#ifndef ZSHUTDOWNDLG_H
#define ZSHUTDOWNDLG_H
#include <QDialog>
#include <QTimer>
#include "ui_zshutdowndlg.h"

class ZShutdownDlg : public QDialog
{
    Q_OBJECT

public:
    ZShutdownDlg(QWidget* parent = 0,Qt::WindowFlags f = 0);
    ~ZShutdownDlg();

private slots:
    void on_btn_start_clicked();
    void onTimerout();
    void onValueChanged(double value);

private:
    Ui::ZShutdownDlg ui;
    QTimer m_timer;
};

#endif // ZSHUTDOWNDLG_H
