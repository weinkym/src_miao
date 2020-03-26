#include "testzwlabeldialog.h"
#include "ui_testzwlabeldialog.h"
#include <QHBoxLayout>

TestZWLabelDialog::TestZWLabelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestZWLabelDialog)
{
    ui->setupUi(this);
    m_textLabel = new ZWLabelText;
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(m_textLabel);
    ui->widgetText->setLayout(hLayout);
}

TestZWLabelDialog::~TestZWLabelDialog()
{
    delete ui;
}

void TestZWLabelDialog::on_pushButton_clicked()
{
//    m_textLabel->setMaxRowText(5,200,ui->textEdit->toPlainText(),"...");
    m_textLabel->setMaxRowText(ui->textEdit->toPlainText(),200,3);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    this->adjustSize();
}
