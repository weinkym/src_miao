#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QWebChannel>
#include "zwlog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_view = new ZWWebengineView;
//    QHBoxLayout *hLayout = new QHBoxLayout;
//    hLayout->addWidget(m_view);
//    ui->tabWidget->currentWidget()->setLayout(hLayout);
//    m_view->load(QUrl("http://www.yidai.com/user/login/"));
//    m_view->startLogin();

    m_bridgeObject = new ZWJSBridgeObject;
    ui->tabWidget->insertTab(0,m_bridgeObject->getView(),QIcon(""),"TEST");

//    m_textView = new QWebEngineView;
//    ui->tabWidget->insertTab(0,m_textView,QIcon(""),"TEST");

//    QWebChannel *channel = new QWebChannel(this);
////    m_bridgeObject = new ZWJSBridgeObject(this);
//    channel->registerObject("ZTESTWebEngineView", this);
//    m_textView->page()->setWebChannel(channel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    m_view->page()->runJavaScript("alert(\"你好，我是一个警告框！\")");
//    QFile file("/Users/miaozw/Documents/TEMP/test.js");

//    if(!file.open(QIODevice::ReadOnly))
//    {
//        return;
//    }
//    QByteArray data = file.readAll();
//    file.close();
//    qDebug()<<"data";
//    qDebug()<<data;
//    m_view->page()->runJavaScript(QString(data));

}

void MainWindow::on_pushButton_2_clicked()
{
//    m_view->page()->load(QUrl("https://www.yidai.com/borrowesharemy/index/"));
//    m_view->startLogin();
//    m_view->startLogin();

}

void MainWindow::on_pbLoadUrl_clicked()
{
    m_bridgeObject->load(QUrl(ui->lineEditUrl->text()));
}

void MainWindow::on_pbRunJS_clicked()
{
//    QString jsString;

//    jsString.append(m_view->m_webchannelJSString);
//    jsString.append("\n");
//    jsString.append(ui->textEditJS->toPlainText());
//    jsString.append("\n");
    m_bridgeObject->runJavaScript(ui->textEditJS->toPlainText());

//    m_bridgeObject->onAmountCallback("1,003.51","本月最近待收：2018-07-02&nbsp;&nbsp;&nbsp;金额：￥",
//                                  "1,003.51","本月最近待收：2018-07-02&nbsp;&nbsp;&nbsp;金额：￥");
}

void MainWindow::testToJS()
{
    QString str("AAAAAAAAAA");
    ZW_VALUE_LOG_INFO(str);
}

void MainWindow::testToJS2(const QString &test, int number)
{
    ZW_VALUE_LOG_INFO(test);
    ZW_VALUE_LOG_INFO(number);

}

int MainWindow::testToJS3(const QString &test, int number)
{
    ZW_VALUE_LOG_INFO(test);
    ZW_VALUE_LOG_INFO(number);
    return 111;
}

QString MainWindow::testToJS4(const QString &test, int number)
{
    ZW_VALUE_LOG_INFO(test);
    ZW_VALUE_LOG_INFO(number);
    return "111";
}



void MainWindow::on_pushButtonAuto_clicked()
{
    m_bridgeObject->start();
}
