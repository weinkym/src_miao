#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <QDebug>
#include "zshadowengine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);


    qreal pi = 3.1415926;
    qreal angle = 60;
    qreal height = 50;
    qreal width = 10;
    qreal d_height = width / qSin(getAgnle(angle/2));
    QRect rect(106,106,400,400);

    {
        ZShadowEngine engine;
        engine.setRadius(width);
        engine.setColor(QColor("#FF0000"));
        engine.painterShadow(&painter,rect,angle,height,4,QColor("#FFFFFF"));
        return;
    }

    painter.drawRect(rect);

    int h_len = qTan(angle * pi / 2 / 180) * height;

    QPointF pos1(rect.center().x() - h_len,rect.y());
    QPointF pos2(rect.center().x(),rect.y() - height);
    QPointF pos3(rect.center().x() + h_len,rect.y());


    QLineF tLine(pos1,pos2);
    {
//        painter.drawLine(tLine);
    }

    qreal tempAngle = (angle / 2 + 90) / 2;
    qreal len_1D1 = width / qSin(getAgnle(tempAngle));
//    qreal len_1D1 =60;
    tLine.setAngle(tLine.angle() + tempAngle);
    tLine.setLength(len_1D1);

    QPointF pos1D(tLine.p2());

    tempAngle = angle / 2;
    qreal len_2D2 = width / qSin(getAgnle(tempAngle));
    QPointF pos2D(pos2.x(),pos2.y() - len_2D2);

    {
//        painter.drawLine(pos1D,pos2D);
    }

    tempAngle = (angle / 2 + 90) / 2;
    tLine = QLineF(pos3,pos2);
    tLine.setAngle(tLine.angle() - tempAngle);
    tLine.setLength(len_1D1);
    QPointF pos3D(tLine.p2());
    {
//        painter.drawLine(pos3D,pos2D);
    }

    QPolygonF polygon1;
    polygon1.append(pos1);
    polygon1.append(pos2);
//    polygon.append(pos3);
//    polygon.append(pos3D);
    polygon1.append(pos2D);
    polygon1.append(pos1D);
    polygon1.append(pos1);

    QPolygonF polygon2;
    polygon2.append(pos3);
    polygon2.append(pos2);
    polygon2.append(pos2D);
    polygon2.append(pos3D);
    polygon2.append(pos3);

    {
        painter.setPen(Qt::NoPen);
        tLine = QLineF(pos1D,pos2D);
        tLine.setAngle(tLine.angle() - 90);
        tLine.setLength(width);
        qreal r = 0.11;
        QLinearGradient linearGradient(tLine.p2(),tLine.p1());
        linearGradient.setColorAt(0,QColor("#FF0000"));
        linearGradient.setColorAt(1,QColor(255,255,255,0));
        painter.setBrush(QBrush(linearGradient));
        painter.drawPolygon(polygon1);
    }
    {
        painter.setPen(Qt::NoPen);
        tLine = QLineF(pos3D,pos2D);
        tLine.setAngle(tLine.angle() + 90);
        tLine.setLength(width);
        qreal r = 0.11;
        QLinearGradient linearGradient(tLine.p2(),tLine.p1());
        linearGradient.setColorAt(0,QColor("#FF0000"));
        linearGradient.setColorAt(1,QColor(255,255,255,0));
        painter.setBrush(QBrush(linearGradient));
        painter.drawPolygon(polygon2);
    }

    ZShadowEngine engine;
    engine.setColor(QColor("#FF0000"));
    engine.setRadius(width);
    QRect shadowRect(rect.x() - width,rect.y()-width,rect.width() + width * 2,rect.height() + width * 2);
    engine.painterShadow(&painter,shadowRect);


//    painter.drawEllipse(QRectF(pos1D.x(),pos1D.y() , 10,10));


//    QPoint pos3(rect.center().x() + h_len,rect.y());

//    QPoint pos1(66,66);
//    QPoint pos2(200,200);
//    int len = 20;
//    QLineF line(pos1,pos2);
//    painter.drawLine(line);
//    {
//        QLineF vLine = line;
//        vLine.setAngle(line.angle() + 90);
//        vLine.setLength(len);
////        QBrush bursh(QColor("#FF00FF80"));
//        painter.drawLine(vLine);
//        qreal len_mind = 10 / qSin(0.1);
//        qDebug()<<qSin(3.1415 * 30 / 180);
//        qDebug()<<line.angle();

//        QLineF mid_line(line.p1(),QPointF(line.x1(),line.y1()+len_mind));
//        painter.drawLine(mid_line);

    //    }
}

qreal MainWindow::getAgnle(qreal angle)
{
    return angle * 3.1415926 / 180;
}
