#ifndef ZPOSCOLORDLG_H
#define ZPOSCOLORDLG_H
#include <QDialog>
#include <QTimer>

class QLabel;
class Ui_ZPosColorDlg;
class ZPosColorDlg : public QDialog
{
   Q_OBJECT

public:
    ZPosColorDlg(QWidget* parent = 0,Qt::WindowFlags f = 0);
    ~ZPosColorDlg();

private slots:
    void updateColor();
    void onBtnClicked();

private:
    Ui_ZPosColorDlg* ui;
    QTimer m_timer;
};


#endif // ZPOSCOLORDLG_H
