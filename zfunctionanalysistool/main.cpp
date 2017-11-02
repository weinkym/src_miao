#include "mainwindow.h"
#include <QApplication>
#include <QRegExp>
#include <QDebug>
#include "zanalyzer.h"

QStringList test(const QString &str)
{
    QStringList strList;
    QRegExp rx("\\[[^\\]]+\\]");
    int pos = 0;
    qDebug()<<__LINE__;
    while((pos = rx.indexIn(str, pos)) != -1)
    {
        qDebug()<<rx.capturedTexts().at(0);
        QString capturedText = rx.capturedTexts().at(0);
        capturedText = capturedText.mid(1,capturedText.length()-2);
        qDebug()<<capturedText;
        strList.append(capturedText);
        pos += rx.matchedLength();
    }
    qDebug()<<__LINE__;
    return strList;
}

void test()
{
    ZAnalyzer analyzer;
    analyzer.analye("/Users/miaozw/Library/Application Support/obs-studio/logs/2017-11-02 09-36-26.txt");
    //
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString str("info:[q]info: ===LJLIVE===20171102 09:35:00 [Enter][function:virtual OBSApp::~OBSApp()][140735814095808] [file = base/clogsetting.cpp ,Line = 7]");
    QStringList strList = test(str);
    test();
    return -1;

    MainWindow w;
    w.show();

    return a.exec();
}
