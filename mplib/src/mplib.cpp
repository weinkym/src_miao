#include <QDir>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QDate>
#include <QTextEdit>
#include "mplib.h"
static QTextEdit* g_textEdit = NULL;

void ZMsgHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QDir dir(qApp->applicationDirPath());
    if(!dir.exists("log"))
        dir.mkdir("log");
    dir.cd("log");

    QString strType = "";
    switch(type)
    {
    case QtDebugMsg:
    {
        strType = "QtDebugMsg";
        break;
    }
    case QtWarningMsg:
    {
        strType = "QtWarningMsg";
        break;
    }
    case QtCriticalMsg:
    {
        strType = "QtCriticalMsg";
        break;
    }
    case QtFatalMsg:
    {
        strType = "QtFatalMsg";
        break;
    }
    default:
        break;
    }
    QString strLog = QString("<%1:%2>%3:%4")
            .arg(qApp->applicationName())
            .arg(strType)
            .arg(QTime::currentTime().toString())
            .arg(msg);
    if(g_textEdit)
    {
        g_textEdit->append(strLog);
    }
    else
    {
        QString logfile = QDate::currentDate().toString("yyyyMMdd");
        QFile file(dir.filePath(QString("%1.log").arg(logfile)));
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream ts(&file);
            ts << strLog << endl;
        }
    }
}


void ZSetMsgHandlerDevice(QTextEdit*textEdit)
{
    g_textEdit = textEdit;
}


Mplib::MpStaticMethod::MpStaticMethod()
{
    //
}

Mplib::MpStaticMethod::~MpStaticMethod()
{
    //
}

QList<int> Mplib::MpStaticMethod::getListInt(int count, int min, int max)
{
    QList<int> result;
    qsrand(QTime::currentTime().second());
    if(min < max)
    {
        int temp = max - min + 1;
        for(int i = 0; i < count; ++i)
        {
            result.append(min + qrand() % temp);
        }
    }
    return result;
}

QList<int> Mplib::MpStaticMethod::getListInt(int min, int max)
{
    QList<int> result;
    for(int i = min; i <= max; ++i)
    {
        result.append(i);
    }
    return result;
}

QString Mplib::MpStaticMethod::getSizeString(qint64 size, int prec)
{
    qDebug()<<"TTTTTTTTTTTTT"<<size;
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

QColor Mplib::MpStaticMethod::getCursorPosColor()
{
    int x = QCursor::pos().x();
    int y = QCursor::pos().y();
    QColor color;
    QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(), x, y, 1, 1);
    if (!pixmap.isNull())
    {
        QImage image = pixmap.toImage();
        if (!image.isNull())
        {
            if (image.valid(0, 0))
            {
                color = image.pixel(0, 0);
                QRgb rgb = image.pixel(0, 0);
                int temp = qAlpha(rgb);
                if(temp != 255)
                {
                    qDebug()<<"YYYYYYYYYYYYYYY"<<temp;
                }
            }
        }
    }
    return color;
}

QString Mplib::MpStaticMethod::convertToPinyin(const QString &text, bool initial)
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
        qDebug()<<"s_unicodePinYinsMap="<<s_unicodePinYinsMap.count();
        qDebug()<<"s_hanziPinYinsMap="<<s_hanziPinYinsMap.count();
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
