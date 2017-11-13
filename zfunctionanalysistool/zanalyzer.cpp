#include "zanalyzer.h"
#include <QFile>
#include <QDebug>
#include "zlogutil.h"

ZAnalyzer::ZAnalyzer()
{

}

ZAnalyzer::~ZAnalyzer()
{

}

void ZAnalyzer::analye(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    m_dataMap.clear();
    QString checkKey1("Enter");
    QString checkKey2("Leave");
    while(!file.atEnd())
    {
        QString lineString = file.readLine();
        if(!(lineString.contains(checkKey1) || lineString.contains(checkKey2)))
        {
            continue;
        }
        QStringList strList = getLineMatchContent(lineString);
        if(strList.isEmpty())
        {
            continue;
        }
        if(strList.count() > 3 && strList.first() == checkKey1)
        {
            const QString &funName = strList.at(1);
            const QString &threadIdString = strList.at(2);
            QString key = funName+"_"+threadIdString;
            if(!m_dataMap.contains(key))
            {
                m_dataMap.insert(key,Info(key));
            }
            m_dataMap[key].count++;
        }
        if(strList.count() > 3 && strList.first() == checkKey2)
        {
            const QString &funName = strList.at(1);
            const QString &threadIdString = strList.at(2);
            QString timeConsumingString = strList.at(3);
            timeConsumingString.remove("ms");
            int time_consuming = timeConsumingString.toInt();
            QString key = funName+"_"+threadIdString;
            if(!m_dataMap.contains(key))
            {
                Z_LOG_WARNING(QString("key=%1 is not contains").arg(key));
            }
            else
            {
                m_dataMap[key].count--;
                m_dataMap[key].time_consuming_all+=time_consuming;
                if(m_dataMap[key].time_consuming_max < time_consuming)
                {
                    m_dataMap[key].time_consuming_max = time_consuming;
                }
            }
        }
    }
    printData();
}

QStringList ZAnalyzer::getLineMatchContent(const QString &str)
{
    QStringList strList;
    QRegExp rx("\\[[^\\]]+\\]");
    int pos = 0;
//    Z_LOG_LINE;

    while((pos = rx.indexIn(str, pos)) != -1)
    {
        QString capturedText = rx.capturedTexts().at(0);
        capturedText = capturedText.mid(1,capturedText.length()-2);
        Z_LOG_DEBUG(capturedText);
        strList.append(capturedText);
        pos += rx.matchedLength();
    }
    return strList;
}

void ZAnalyzer::printData()
{
    QMapIterator<QString,Info> iter(m_dataMap);
    while(iter.hasNext())
    {
        iter.next();
        QString logInfo = QString("fun=%1,count=%2,time_consuming_all=%3,time_consuming_max=%4")
                .arg(iter.value().name).arg(iter.value().count).arg(iter.value().time_consuming_all)
                  .arg(iter.value().time_consuming_max);
        Z_LOG_WARNING(logInfo);
        if(iter.value().count > 0)
        {
            Z_LOG_WARNING(logInfo);
        }
    }
}

ZAnalyzer::Info::Info(const QString &name)
{
    this->name = name;
    this->count = 0;
    this->time_consuming_all = 0;
    this->time_consuming_max = 0;
}

ZAnalyzer::Info::Info()
{
    this->name = "default";
    this->count = 0;
    this->time_consuming_all = 0;
    this->time_consuming_max = 0;
}
