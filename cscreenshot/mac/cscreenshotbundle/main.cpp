#include <QApplication>
#include "cscreenshotmanager.h"
#include "cscreenshotobject.h"

#include <QUuid>
#include <QFile>
#include <QDir>
QFile file;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{


    QString prefix;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        prefix = "Debug";
        break;
    case QtInfoMsg:
        prefix = "Info";
        break;
    case QtWarningMsg:
        prefix = "Warning";
        break;
    case QtCriticalMsg:
        prefix = "Critical";
        break;
    case QtFatalMsg:
        prefix = "Fatal";
        abort();
    }
    QString contentString = QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss ")+prefix + QString("content=%1,file=%2,function=%3,line=%4\n").arg(localMsg.constData()).arg(context.file).arg(context.function).arg(context.line);

    if(file.isOpen())
    {
        file.write(contentString.toLatin1());
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir dir(qApp->applicationDirPath());
    dir.cd("../../../");
    QString logPath = QString("%1/cscreenshot.log").arg(dir.absolutePath());
    file.setFileName(logPath);
    qDebug()<<"logPath="<<logPath;
    if(!file.open(QIODevice::WriteOnly))
    {
        //
    }
    file.write("----------------start screenshot");
//    qInstallMessageHandler(myMessageOutput);

    C_SCREENSHOT_LOG_TEST;
    CScreenShotObject obj;

    QStringList arguments = a.arguments();
    qDebug()<<arguments.join("-------");
    bool needCheck = false;
    if(needCheck)
    {
        if(arguments.count() < 2)
        {
            qDebug()<<QString("input start key");
            return 1;
        }

        QString startKey = "c8a55807-c95a-490e-91c0-74bdbc129904";
        if(arguments.at(1) != startKey)
        {
            qDebug()<<QString("start key is error");
            return 2;
        }
        if(obj.isStart(startKey))
        {
            return 3;
        }
    }
    obj.startScreenShot();
    C_SCREENSHOT_LOG_TEST;

    file.close();
    C_SCREENSHOT_LOG_TEST;
    return a.exec();
}
