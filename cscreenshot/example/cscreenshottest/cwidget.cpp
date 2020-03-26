#include "cwidget.h"
#include "ui_cwidget.h"
#include <QProcess>
#include <QPixmap>
#include <QDir>

CWidget::CWidget(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::CWidget)
    ,m_process(NULL)
{
    ui->setupUi(this);
    connect(ui->btnClose,SIGNAL(clicked(bool)),this,SLOT(close()));
}

CWidget::~CWidget()
{
    delete ui;
}

void CWidget::on_btnStart_clicked()
{
    if(m_process == NULL)
    {
        m_process = new QProcess(this);
//        connect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
//        connect(m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
        connect(m_process, SIGNAL(readyRead()), this, SLOT(readFromClient()));
    }
//    m_process->start("/Users/bitbrothers/work/others/src/workroot/src_miao/cscreenshot/example/cscreenshotbundle/TEMP/cscreenshotbundle.app/Contents/MacOS/cscreenshotbundle");
//    m_process->start("/Users/bitbrothers/work/others/src/workroot/src_miao/cscreenshot/example/cscreenshotbundle/TEMP/cscreenshotbundle.app");
    QString dirPath = QApplication::applicationDirPath();
    QDir dir(dirPath);
    dir.cd("../Library");
    QString filePath = dir.path()+"/cscreenshotbundle.app";
    ui->textEdit->append(QString("filePath = %1").arg(filePath));
    m_process->start(filePath);
}

void CWidget::readFromClient()
{
    if(m_process == NULL)
    {
        return;
    }

    QByteArray output = m_process->readAllStandardOutput();
    ui->textEdit->append(QString("output = %1").arg(output.size()));
    QPixmap pixmap;
//    pixmap.load(output,"PNG");
    pixmap.loadFromData(output);
    pixmap.setDevicePixelRatio(2);
    ui->label->setPixmap(pixmap);
    ui->textEdit->append(QString("output = %1").arg(pixmap.isNull() ?"null":"not null"));

}
