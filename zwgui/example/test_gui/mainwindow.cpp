#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include "mac/zwplatformmac.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updatePoints();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

//    static QList<QPointF> points = QList<QPointF>() << QPointF(0, 0) << QPointF(100, 100) << QPointF(200, -100)
//                                                        << QPointF(300, 100) << QPointF(330, -80) << QPointF(350, -70);
    QPainterPath path = ZWGui::createPath(m_points);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 2));
    // 绘制 path
    painter.translate(40, 130);
    painter.drawPath(path);
}

void MainWindow::on_pushButton_clicked()
{
//    ZW_LOG::initLog("/Users/miaozw/Documents/TEMP/test.log");
//    ZW_LOG_DEBUG("asdfasdfadsf");
//    ZW_LOG_FUNCTION;
    //    ZW_LOG_INFO(ZWCore::convertToPinyin("缪正伟DFADSF"));
    updatePoints();

}

void MainWindow::updatePoints()
{
    m_points.clear();
    int count = qrand() % 100 + 5;

    QMap<qreal,qreal> dataMap;
    for(int i = 0; i < count; ++i)
    {
        dataMap.insert(qrand() % 100000 * 0.01,qrand() % 200000 * 0.001);
    }
    QMapIterator<qreal,qreal> iter(dataMap);
    while(iter.hasNext())
    {
        iter.next();
        m_points.append(QPointF(iter.key(),iter.value()));
    }
    this->update();
}
