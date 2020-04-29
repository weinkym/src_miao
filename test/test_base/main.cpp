
#include "clogsetting.h"
#include "ctestwidget.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CLogEngine::getInstance()->setFieldTypes(C_LOG::FieldTypeFlags(C_LOG::FIELD_TYPE_LINE | C_LOG::FIELD_TYPE_FUN));
    //    FIELD_TYPE_DATE = 0x00000001,
    //    FIELD_TYPE_DATETIME = 0x00000002,
    //    FIELD_TYPE_LINE = 0x00000004,
    //    FIELD_TYPE_FILE = 0x00000008,
    //    FIELD_TYPE_LEVEL = 0x00080010,
    //    FIELD_TYPE_FUN = 0x00080020,
    //    FIELD_TYPE_THREADID = 0x00080040,
    MainWindow w;
    w.show();
    //    CTestWidget tw;
    //    tw.show();
    //    QWidget w;
    //    w.show();

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
