#include "zwcore.h"
#include "zwlog.h"
#include <QFile>

QMap<ushort,QStringList> s_unicodePinYinsMap;
QMap<QString,QStringList> s_hanziPinYinsMap;
bool g_initialed_pinyin_map = false;

ZWCore::ZWCore()
{
}

static void initializePinyinMap()
{
    if(g_initialed_pinyin_map)
    {
        return;
    }
    QFile file(":/other/hanzipinyin.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        ZW_LOG_WARNING(QString("open file is error,filename=%1,error=%2").arg(file.fileName()).arg(file.errorString()));
        return;
    }
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

    ZW_LOG_INFO(QString("first initial s_unicodePinYinsMap=%1,s_hanziPinYinsMap=%2")
                .arg(s_unicodePinYinsMap.count()).arg(s_hanziPinYinsMap.count()));
    g_initialed_pinyin_map = true;
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
    initializePinyinMap();
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

QStringList ZWCore::convertToPinyin(const QChar &c, bool initial)
{
    return convertToPinyin(c.unicode(),initial);
}

QStringList ZWCore::convertToPinyin(ushort unicode, bool initial)
{
    initializePinyinMap();
    if(s_unicodePinYinsMap.contains(unicode))
    {
        QStringList resultList;
        if(initial)
        {
            QStringList pinyinList = s_unicodePinYinsMap.value(unicode);
            int count = pinyinList.length();
            for(int i = 0; i < count; ++i)
            {
                const QString &pinyin = pinyinList.at(i);
                if(!pinyin.isEmpty())
                {
                    resultList.append(pinyin.at(0));
                }
            }
        }
        else
        {
            resultList = s_unicodePinYinsMap.value(unicode);
        }
        return s_unicodePinYinsMap.value(unicode);
    }
    return QStringList(QString(QChar(unicode)));
}

QString ZWCore::getBoolString(bool boolValue)
{
    return boolValue ? "TRUE":"FALSE";
}

//UTF-8是一种多字节编码的字符集，表示一个Unicode字符时，它可以是1个至多个字节，在表示上有规律：
//1字节：0xxxxxxx
//2字节：110xxxxx 10xxxxxx
//3字节：1110xxxx 10xxxxxx 10xxxxxx
//4字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//但很少会遇到4字节的
bool ZWCore::isUTF8(const char *srcBuffer, long size)
{
    bool ret = true;
    unsigned char* start = (unsigned char*)srcBuffer;
    unsigned char* end = (unsigned char*)srcBuffer + size;
    while(start < end)
    {
        if(*start < (0x80))
        {
            // (10000000): 值小于0x80的为ASCII字符
            start++;
        }
        else if(*start < (0xC0))
        {
            // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
            ret = false;
            break;
        }
        else if(*start < (0xE0))
        {
            // (11100000): 此范围内为2字节UTF-8字符
            if (start >= end - 1)
            {
                break;
            }
            if ((start[1] & (0xC0)) != 0x80)
            {
                ret = false;
                break;
            }
            start += 2;
        }
        else if (*start < (0xF0))
        {
            // (11110000): 此范围内为3字节UTF-8字符
            if (start >= end - 2)
            {
                break;
            }
            if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
            {
                ret = false;
                break;
            }
            start += 3;
        }
        else if (*start < (0xF8))
        {
            // (11111000): 此范围内为4字节UTF-8字符
            if (start >= end - 3)
            {
                break;
            }
            if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80 || (start[3] & (0xC0)) != 0x80)
            {
                ret = false;
                break;
            }
            start += 4;
        }
        else
        {
            ret = false;
            break;
        }
    }
    return ret;
}
