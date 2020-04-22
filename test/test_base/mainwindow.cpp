#include "mainwindow.h"
#include "clogsetting.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QSvgRenderer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widget_click, SIGNAL(sigClicked(QUuid)), this, SLOT(onTestSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //    CTestDialog *dlg = new CTestDialog(CBaseDialog::SHOW_TYPE_NORMAL_MODAL);
    //    dlg->setText("M1");
    //    QTimer::singleShot(1000, this, []() {
    //        CTestDialog *dlg = new CTestDialog(CBaseDialog::SHOW_TYPE_HALF_MODAL);
    //        dlg->setText("M2");
    //        dlg->move(0, 0);
    //        dlg->startShow();
    //    });
    //    dlg->startShow();

    onTestSlot();
}

void MainWindow::onTestSlot()
{
    C_LOG_FUNCTION;
    //    QSvgRenderer *svg_render = new QSvgRenderer(QString(":/res/test.svg"));
    //    QPixmap *pixmap = new QPixmap(128, 128);
    //    pixmap->fill(Qt::transparent);
    //    QPainter painter(pixmap);
    //    svg_render->render(&painter, QRectF(0, 0, pixmap->width(), pixmap->height()));
    //    pixmap->setDevicePixelRatio(2);
    //    ui->label_2->setMinimumSize(pixmap->size() / 2);
    //    ui->label_2->setMaximumSize(pixmap->size() / 2);
    //    ui->label_2->setPixmap(*pixmap);
    //    ui->label_2->setScaledContents(false);
    QPixmap pixmap = CUIUtils::getSvgFilePixmap(QString(":/res/test.svg"), QSize(256, 256), 2);
    ui->label_2->setMinimumSize(pixmap.size() / 2);
    ui->label_2->setMaximumSize(pixmap.size() / 2);
    ui->label_2->setPixmap(pixmap);
}
