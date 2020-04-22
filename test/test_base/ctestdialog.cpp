#include "ctestdialog.h"
#include "ui_ctestdialog.h"

CTestDialog::CTestDialog(CBaseDialog::ShowType showType, QWidget *parent, Qt::WindowFlags f)
    : CBaseDialog(showType, parent, f)
    , ui(new Ui::CTestDialog)
{
    ui->setupUi(this);
}

CTestDialog::~CTestDialog()
{
    delete ui;
}

void CTestDialog::setText(const QString &text)
{
    ui->pushButton->setText(text);
}

void CTestDialog::on_pushButton_clicked()
{
    this->accept();
    this->close();
}
