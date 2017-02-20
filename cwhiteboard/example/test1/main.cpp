#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDir>

void addTranslator(QApplication &a,const QString &translatorName,const QString &path)
{
    QTranslator *translator = new QTranslator;
    bool isLoaded = translator->load(translatorName, path);
    if(isLoaded)
    {
        a.installTranslator(translator);
    }
    qDebug()<<QString("load result=%1").arg(isLoaded?"ok":"error");

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir dir("../../../../../../");
    qDebug()<<dir.absolutePath();
    addTranslator(a,"cwhiteboardd_zh_CN",dir.absolutePath());
//    addTranslator(a,"qt_widgets_zh_CN.qm","../../../../");
    addTranslator(a,"qt_zh_CN.qm","../../../../");


    MainWindow w;
    w.show();

    return a.exec();
}
