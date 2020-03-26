#include "zcatlogwidget.h"
#include "ui_zcatlogwidget.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

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
    if(ui->checkBoxFunction->isChecked())
    {
        doUpdateFunction();
        return;
    }
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

struct LogData{
    int line = 0;
    int count = 0;
};

void ZCatlogWidget::doUpdateFunction()
{
    ui->textEdit->clear();
    ui->textEdit->append("start");
    QFile file(ui->lineEditPath->text());
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QTextStream textStream(&file);
    int line = 0;
    Qt:: CaseSensitivity cs = ui->checkBoxCaseSensitivity->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    bool isRegExp = true;

    //    QRegExp rx("(((http|ftp|https)://)|(www\\.))+([\\S]+\\.)+([a-zA-Z0-9]{2,6})+(:[0-9]{1,4})*(/[\\S]*)*");
    //    QRegExp rx("((\([5-9][0-9]{1,}ms\))|\(([0-9][0-9]{1,}ms\)))");
    QString key = ui->lineEditKey->text();
    ui->textEdit->append(QString("key=%1").arg(key));

//    QRegExp rx(key);
    QRegExp rx("\\[.*\\]");

    QString startKey = "[Enter]";
    QString endKey = "[Leave]";


    QMap<QString,LogData> funMap;
    rx.setMinimal(true);
    int row = 1;
//    static int s_test_count_enter = 0;
//    static int s_test_count_leave = 0;
    while(!textStream.atEnd())
    {
        line++;
        QString lineString = textStream.readLine();
        lineString = lineString.replace(QString("<"),QString("&lt;"))
                .replace(QString(">"),QString("&gt;"))
                .replace(QString("&nbsp;"),QString(" "));
        QStringList tempString = regExpStringList(rx,lineString);
//        qDebug()<<"tempString"<<tempString;

        if(tempString.isEmpty())
        {
            continue;
        }
        if(tempString.count() < 2)
        {
            continue;
        }
        QString content = tempString.at(1);
        LogData obj = funMap.value(content);
//        int addCount = 0;

        if(tempString.contains(startKey))
        {
//            content.remove(startKey);
            obj.count++;
            obj.line = line;
//            if(tempString.contains("[function:void __thiscall ft2_source::prepareUpdate(void)]"))
//            {
//                s_test_count_enter++;
//            }
        }
        else if(tempString.contains(endKey))
        {
            content.remove(endKey);
            obj.count--;
//            if(tempString.contains("[function:void __thiscall ft2_source::prepareUpdate(void)]"))
//            {
//                s_test_count_leave++;
//            }
        }
        int leave = 0;
        int enter = 0;
        for(auto obj:tempString)
        {
            if(obj.contains(startKey))
            {
                enter++;
            }
            if(obj.contains(endKey))
            {
                leave++;
            }
        }
        if(leave + enter > 1)
        {
            qDebug()<<"line=="<<line;
        }
//        if(obj.count >1 && tempString.contains("[function:void __thiscall ft2_source::prepareUpdate(void)]"))
//        {
//            qDebug()<<"line=="<<line;
//        }
//        obj.count += addCount;
        funMap.insert(content,obj);
    }
//    qDebug()<<"line"<<s_test_count_enter;
//    qDebug()<<"line"<<s_test_count_leave;
    QMapIterator<QString,LogData> iter(funMap);
    while(iter.hasNext())
    {
        iter.next();
        if(iter.value().count != 0)
        {
            ui->textEdit->append(QString("fun=%1,count=%2,line=%3").arg(iter.key()).arg(iter.value().count).arg(iter.value().line));
        }
    }
    ui->textEdit->append("end");
}

QStringList ZCatlogWidget::regExpStringList(const QRegExp &rx, const QString &source)
{
    QStringList capList;
    int pos = 0;
    while ((pos = rx.indexIn(source, pos)) != -1)
    {
        capList << rx.cap(0);
        pos += rx.matchedLength();
    }
    return capList;
}

void ZCatlogWidget::updateFilePathWithLog()
{
#ifdef Q_OS_MAC
    QString dirName = "/Users/miaozw/Library/Application Support/yxtliveshow/ljobs/logs";
#endif

    QDir dir(dirName);
    QDateTime dateTime;
    QString filePath;
    foreach(QFileInfo file, dir.entryInfoList())
    {
        if(file.fileName() == "." || file.fileName() == ".")
            continue;
        if(file.isDir())
            continue;

        QDateTime curDateTime = QDateTime::fromString(file.baseName(),"yyyyMMddhhmmsszzz");
        if(curDateTime.isValid() && !curDateTime.isNull() && curDateTime > dateTime)
        {
            filePath = file.absoluteFilePath();
        }
    }
    if(!filePath.isEmpty())
    {
        ui->lineEditPath->setText(filePath);
    }
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

void ZCatlogWidget::on_pushButton_clicked()
{
    updateFilePathWithLog();
}
