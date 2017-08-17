#include "mainwindow.h"
#include <QApplication>
#include "cxmlreader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CXmlReader xmlReader;
//    xmlReader.read("/Users/miaozw/Documents/pkg/ljlive_zh_CN.ts");
//    xmlReader.save("/Users/miaozw/Documents/pkg/ljlive_zh_EG.ts");

    xmlReader.convert("/Users/miaozw/Documents/pkg/ljlive_zh_CN.ts"
                      ,"/Users/miaozw/Documents/pkg/language.txt",
                      "/Users/miaozw/Documents/pkg/tsout",
                      "/Users/miaozw/Documents/pkg/locale");
    return -1;
    MainWindow w;
    w.show();

    return a.exec();
}
