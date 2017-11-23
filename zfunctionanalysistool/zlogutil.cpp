#include "zlogutil.h"
#include <QFile>
#include <QDateTime>

QFile g_log_file;
ZLogUtil::ZLogUtil()
{

}

ZLogUtil::~ZLogUtil()
{

}

void debugMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString typeName;
    switch (type) {
    case QtDebugMsg:
        typeName = "QtDebugMsg";
        break;
    case QtInfoMsg:
        typeName = "QtInfoMsg";
        break;
    case QtWarningMsg:
        typeName = "QtWarningMsg";
        break;
    case QtCriticalMsg:
        typeName = "QtCriticalMsg";
        break;
    case QtFatalMsg:
        typeName = "QtFatalMsg";
        abort();
    }
    if(!g_log_file.isOpen())
    {
        g_log_file.open(QIODevice::WriteOnly | QIODevice::Append);
    }
    QByteArray logContent = QString("[%1]").arg(QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss")).toLocal8Bit() +
            QString("[%1]").arg(typeName).toLocal8Bit() +
            localMsg + "\n";
    if(g_log_file.isOpen())
    {
        g_log_file.write(logContent);
    }
    fprintf(stderr,logContent.constData());
}

void setLogFileName(const QString &fileName)
{
    if(g_log_file.isOpen())
    {
        g_log_file.close();
    }
    g_log_file.setFileName(fileName);
}
