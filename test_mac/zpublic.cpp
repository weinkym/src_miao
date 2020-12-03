#include "zpublic.h"
#include <QApplication>
#include <QEventLoop>
#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QtCore/QThread>
#include <QtCore/QTimer>
//#include <process.h>
#include <unistd.h>

struct PD_CPU
{
    qreal user;
    qreal sys;
    qreal idle;
};

struct PD_APP
{
    qreal cpu;
    qint64 mem;
};

PD_CPU parseCPU(const QByteArray &content)
{
    qDebug() << content;
    PD_CPU obj;
    QList<qreal> numList = ZPublic::getStringNums(content);
    if(numList.count() == 3)
    {
        obj.user = numList.at(0);
        obj.sys = numList.at(1);
        obj.idle = numList.at(2);
    }
    return obj;
}

qreal getValue(const QString &str)
{
    qDebug() << "=========str" << str;
    QList<qreal> nums = ZPublic::getStringNums(str);
    if(nums.isEmpty())
    {
        return 0.0;
    }
    return nums.first();
}

PD_APP parseAPP(const QString &lineKey, const QString &lineData)
{
    PD_APP obj;

    QStringList keys = lineKey.split(" ", QString::SkipEmptyParts);
    QStringList datas = lineData.split(" ", QString::SkipEmptyParts);
    qDebug() << "=========lineKey" << lineKey << keys.count();
    qDebug() << "=========lineData" << lineData << datas.count();
    do
    {
        if(keys.count() != datas.count())
        {
            break;
        }
        for(int i = 0; i < keys.count(); ++i)
        {
            QString key = keys.at(i);
            if(key == "%CPU")
            {
                obj.cpu = getValue(datas.at(i));
            }
            if(key == "MEM")
            {
                const QString &valueString = datas.at(i);
                qint64 flag = 1;
                if(valueString.contains("G"))
                {
                    flag = 1024 * 1024 * 1024;
                }
                else if(valueString.contains("M"))
                {
                    flag = 1024 * 1024;
                }
                else if(valueString.contains("K"))
                {
                    flag = 1024;
                }
                obj.mem = flag * getValue(valueString);
            }
        }
    } while(0);

    return obj;
}

void ZPublic::testAll()
{
    {
        //        parseCPU("CPU usage: 5.88% user, 6.40% sys, 87.71114134afasdf.");
        //        getStringNums("CPU usage: 5.88% user, 6.40% sys, 87.71114134afasdf.3");
    }
    if(1)
    {
        PD_CPU cpuObj;
        PD_APP appObj;
        pid_t pid = getpid();
        qDebug() << "=========pidpid" << pid;
        QString cmd = QString("top -pid %1 -l1").arg(pid);
        qDebug() << "=========cmd" << cmd;
        QProcess p;
        p.start(cmd);
        p.waitForFinished();
        p.waitForReadyRead();
        QString data = p.readAllStandardOutput();
        QStringList lines = data.split('\n');
        for(int i = 0; i < lines.count(); ++i)
        {
            const QString &line = lines.at(i);
            QString key = "CPU usage: ";
            if(line.startsWith(key))
            {
                cpuObj = parseCPU(line.toLatin1());
            }
            else if(line.startsWith("PID    COMMAND  %CPU"))
            {
                if(i < lines.count() - 1)
                {
                    ++i;
                    appObj = parseAPP(line, lines.at(i));
                }
            }
        }
        qDebug() << "=========appObj" << appObj.cpu << appObj.mem;
        qDebug() << "=========cpuObj" << cpuObj.user << cpuObj.sys << cpuObj.idle;
    }
}

QList<qreal> ZPublic::getStringNums(const QString &content)
{
    QList<qreal> numList;
    QString numString;
    bool hasPoint = false;
    for(int i = 0; i < content.length(); ++i)
    {
        QChar c = content.at(i);
        bool isNumEnd = (i == content.length() - 1);

        if(c.isDigit())
        {
            numString.append(c);
        }
        else
        {
            if(c == '.' && !hasPoint && !numString.isEmpty())
            {
                hasPoint = true;
                numString.append(c);
            }
            else
            {
                isNumEnd = true;
            }
        }

        if(isNumEnd && !numString.isEmpty())
        {
            bool ok = false;
            qreal num = numString.toDouble(&ok);
            if(ok)
            {
                numList.append(num);
            }
            numString = "";
            hasPoint = false;
        }
    }
    return numList;
}
