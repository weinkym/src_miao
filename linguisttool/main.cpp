#include "mainwindow.h"
#include <QApplication>
#include "cxmlreader.h"
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = a.arguments();
    if(args.count() != 5)
    {
        qDebug()<<"arg is not 4 count";
//        std::cerr  <<"asdfasdfadsf";
        return -1;
    }
    QString sourceFilePath = args[1];
    QString languageDataFilePath = args[2];
    QString outPath = args[3];
    QString obsFilePath = args[4];
    CXmlReader xmlReader;
//    xmlReader.read("/Users/miaozw/Documents/pkg/ljlive_zh_CN.ts");
//    xmlReader.save("/Users/miaozw/Documents/pkg/ljlive_zh_EG.ts");
    xmlReader.convert(sourceFilePath,languageDataFilePath,outPath,obsFilePath);
//    xmlReader.convert("/Users/miaozw/Documents/pkg/ljlive_zh_CN.ts",
//                      "/Users/miaozw/Documents/pkg/language.txt",
//                      "/Users/miaozw/Documents/pkg/tsout",
//                      "/Users/miaozw/Documents/pkg/locale");
    return -1;
//    MainWindow w;
//    w.show();

    return a.exec();
}
