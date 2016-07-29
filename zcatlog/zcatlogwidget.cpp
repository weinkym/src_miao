#include "zcatlogwidget.h"
#include "ui_zcatlogwidget.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QSettings>
#include <QMessageBox>

ZCatlogWidget::ZCatlogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZCatlogWidget)
{
    ui->setupUi(this);
    ui->lineEditPath->setReadOnly(true);
    ui->lineEditKey->installEventFilter(this);
    //    QSettings settings;
    QSettings settings("MIAOTEST", "MIAOTEST");
    settings.beginGroup("ZCatlogWidget");
    ui->lineEditPath->setText(settings.value("lineEditPath").toString());
    ui->lineEditKey->setText(settings.value("lineEditKey").toString());
    settings.endGroup();
}

ZCatlogWidget::~ZCatlogWidget()
{
    QSettings settings("MIAOTEST", "MIAOTEST");
    settings.beginGroup("ZCatlogWidget");
    settings.setValue("lineEditPath", ui->lineEditPath->text());
    settings.setValue("lineEditKey", ui->lineEditKey->text());
    settings.endGroup();
    delete ui;
}

bool ZCatlogWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->lineEditKey && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);
        if(keyEvent)
        {
            if(keyEvent->key() == Qt::Key_Return
                    || keyEvent->key() == Qt::Key_Enter )
            {
                updateText();
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}

void ZCatlogWidget::updateText()
{
    ui->textEdit->clear();
    QFile file(ui->lineEditPath->text());
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QTextStream textStream(&file);
    int line = 1;
    Qt:: CaseSensitivity cs = ui->checkBoxCaseSensitivity->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    bool isRegExp = ui->checkBoxRegExp->isChecked();
    QStringList serachKeyList = ui->lineEditKey->text().split(";");
    if(serachKeyList.isEmpty())
    {
        return;
    }
    //    QRegExp rx("(((http|ftp|https)://)|(www\\.))+([\\S]+\\.)+([a-zA-Z0-9]{2,6})+(:[0-9]{1,4})*(/[\\S]*)*");
    //    QRegExp rx("((\([5-9][0-9]{1,}ms\))|\(([0-9][0-9]{1,}ms\)))");
    QString key = ui->lineEditKey->text();
    QRegExp rx(key);
    rx.setMinimal(true);
    int row = 1;
    while(!textStream.atEnd())
    {
        QString lineString = textStream.readLine();
        lineString = lineString.replace(QString("<"),QString("&lt;"))
                .replace(QString(">"),QString("&gt;"))
                .replace(QString("&nbsp;"),QString(" "));
        line++;
        bool isOk = false;
        QString destString;
        if(isRegExp)
        {
            isOk =  regExpString(rx,lineString,destString);
        }
        else
        {
            destString = lineString;
            if(destString.contains(key,cs))
            {
                QString replaceString = toHtmlString(key,QColor(Qt::yellow),QColor(Qt::darkRed));
                destString.replace(key,replaceString,cs);
                isOk = true;
            }
        }
        if(isOk)
        {
            QString preFixe =toHtmlString(QString("row:%1 sourceLine=%2 ").arg(row++).arg(line),QColor("#C4E1FF"),QColor(Qt::darkRed));
            ui->textEdit->append(QString("%1 %2").arg(preFixe).arg(destString));
        }
    }
    QString preFixe =toHtmlString(QString("row:%1 sourceLine=%2 ").arg(row++).arg(line),QColor("#CCFF80"),QColor(Qt::darkRed));
    ui->textEdit->append(QString("%1 %2").arg(preFixe).arg("find end"));
}

bool ZCatlogWidget::regExpString(const QRegExp &rx, const QString &source, QString &dest)
{
    QString newString = source;
    //    QRegExp rx("\\[.*\\]");
    //    rx.setMinimal(true);
    QStringList capList;
    int pos = 0;
    while ((pos = rx.indexIn(source, pos)) != -1)
    {
        capList << rx.cap(0);
        pos += rx.matchedLength();
    }
    if(capList.isEmpty())
    {
        return false;
    }
    foreach(QString cap, capList)
    {
        newString.replace(cap, toHtmlString(cap,QColor(Qt::yellow),QColor(Qt::red)));
    }
    dest = newString;
    return true;
}

QString ZCatlogWidget::toHtmlString(const QString &source, const QColor &backgroundColor, const QColor &color)
{
    return QString("<strong style=\"background:%1\"><font color=\"%2\">%3</font></strong>").arg(backgroundColor.name()).arg(color.name()).arg(source);
}

void ZCatlogWidget::on_pushButtonUpdate_clicked()
{
    updateText();
}

void ZCatlogWidget::on_pushButtonLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    ui->lineEditPath->text(),
                                                    tr("Images (*.*)"));
    if(fileName.isEmpty())
    {
        return;
    }
    ui->lineEditPath->setText(fileName);
    updateText();
}

void ZCatlogWidget::on_pushButtonClose_clicked()
{
    this->close();
}
