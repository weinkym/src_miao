#include "mainwindow.h"
#include "clogsetting.h"
#include "ui_mainwindow.h"

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
    CTestDialog *dlg = new CTestDialog(CBaseDialog::SHOW_TYPE_NORMAL_MODAL);
    dlg->setText("M1");
    QTimer::singleShot(1000, this, []() {
        CTestDialog *dlg = new CTestDialog(CBaseDialog::SHOW_TYPE_HALF_MODAL);
        dlg->setText("M2");
        dlg->move(0, 0);
        dlg->startShow();
    });
    dlg->startShow();
}

void MainWindow::onTestSlot()
{
    C_LOG_FUNCTION;
}
