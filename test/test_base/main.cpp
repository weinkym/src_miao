#include "cdialog.h"
#include "cwidget.h"

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //    QWidget w;
    //    //    w.show();

    //    QSize size(400, 400);
    //    CWidget *cw = new CWidget(&w);
    //    cw->setStyleSheet("QWidget{ background:red}");
    //    //    cw->setBaseSize(size);
    //    //    cw->setGeometry(0, 0, 400, 400);
    //    cw->setGeometry(QRect(200, 50, 120, 80));
    //    cw->raise();

    //    w.hide();
    //    w.show();

    //    CDialog dlg(CDialog::SHOW_TYPE_NORMAL);
    //    dlg.show();

    return a.exec();
}
