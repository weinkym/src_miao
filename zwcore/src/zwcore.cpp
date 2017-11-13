#include "zwcore.h"
#include "zwlog.h"
#include <QFile>

ZWCore::ZWCore()
{
}


QString ZWCore::getSizeString(qint64 size, int prec)
{
    int index = 0;
    QString suffix = "B";
    double value = 0.0;
    while(size > 0)
    {
        switch (index) {
        case 0:
        {
            suffix = "B";
            value = size % 1024;
            break;
        }
        case 1:
        {
            suffix = "KB";
            value = value / 1024.0 + size % 1024;
            break;
        }
        case 2:
        {
            suffix = "MB";
            value = value / 1024.0 + size % 1024;
            break;
        }
        case 3:
        {
            suffix = "GB";
            value = value / 1024.0 + size % 1024;
            break;
        }
        case 4:
        {
            suffix = "TB";
            value = value / 1024.0 + size % 1024;
            break;
        }
        case 5:
        {
            suffix = "TB";
            value = value + size * 1024;
            return QString("%1%2").arg(QString::number(value,'f',prec)).arg(suffix);
            break;
        }
        default:
            break;
        }
        size /= 1024;
        index++;
    }
    return QString("%1%2").arg(QString::number(value,'f',prec)).arg(suffix);
}

QString ZWCore::convertToPinyin(const QString &text, bool initial)
{
    static bool isFirst = true;
    static QMap<quint64,QStringList> s_unicodePinYinsMap;
    static QMap<QString,QStringList> s_hanziPinYinsMap;
    if(isFirst)
    {
        isFirst = false;
        QFile file(":/other/hanzipinyin.txt");
        if(file.open(QIODevice::ReadOnly))
        {
            while(!file.atEnd())
            {
                QStringList lineStringList = QString(file.readLine()).split(" ");
               if(lineStringList.count() != 3)
               {
                   continue;
               }
               if(lineStringList.at(0) != "#")
               {
                   continue;
               }
               QString pinyin = lineStringList.at(1);
               QString hanziString = lineStringList.at(2);
               int count = hanziString.count();
               for(int i = 0; i < count; ++i)
               {
                   ushort unicode = hanziString.at(i).unicode();
                   if(s_unicodePinYinsMap.contains(unicode))
                   {
                       QStringList  &existList = s_unicodePinYinsMap[unicode];
                       if(!existList.contains(pinyin))
                       {
                           existList.append(pinyin);
                       }
                   }
                   else
                   {
                       s_unicodePinYinsMap.insert(unicode,QStringList(pinyin));
                   }
                   QString hz =  hanziString.at(i);
                   if(s_hanziPinYinsMap.contains(hz))
                   {
                       QStringList  &existList = s_hanziPinYinsMap[hz];
                       if(!existList.contains(pinyin))
                       {
                           existList.append(pinyin);
                       }
                   }
                   else
                   {
                       s_hanziPinYinsMap.insert(hz,QStringList(pinyin));
                   }
               }
            }
        }
        ZW_LOG_INFO(QString("first initial s_unicodePinYinsMap=%1,s_hanziPinYinsMap=%2")
                    .arg(s_unicodePinYinsMap.count()).arg(s_hanziPinYinsMap.count()));
    }
    QString resutlString;
    int count = text.length();
    for(int i = 0; i < count; ++i)
    {
        ushort unicode = text.at(i).unicode();
        QStringList pinyinList = s_unicodePinYinsMap.value(unicode);
        if(pinyinList.isEmpty())
        {
            resutlString.append(text.at(i));
        }
        else
        {
            QString pinyin = pinyinList.at(0);
            if(initial)
            {
                if(pinyin.isEmpty())
                {
                    resutlString.append(text.at(i));
                }
                else
                {
                    resutlString.append(pinyin.at(0).toUpper());
                }
            }
            else
            {
                resutlString.append(pinyin);
            }
        }
    }
    return resutlString;
}

QString ZWCore::getBoolString(bool boolValue)
{
    return boolValue ? "TRUE":"FALSE";
}
