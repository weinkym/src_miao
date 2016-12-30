#include <QDir>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QDate>
#include <QTextEdit>
#include <qmath.h>
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


void Mplib::MpStaticMethod::convertToMosaic(int mosaicSize, QImage &image)
{
    qDebug()<<QString("line=%1").arg(image.format())<<image.format();

    if(mosaicSize <= 1)
    {
        return;
    }
    int width = image.width();
    int height = image.height();
    uchar* imageBits = image.bits();
    int bytePerLine=image.bytesPerLine();
    qDebug()<<QString("width=%1,height=%2,bytePerLine=%3").arg(width).arg(height).arg(bytePerLine)<<image.byteCount();
    int indexR=0;
    int indexG=0;
    int indexB=0;
    int r=0;
    int g=0;
    int b=0;

    int i = 0;
    int j = 0;
    while(i < height)
    {
        j = 0;
        int maxRow = mosaicSize;
        maxRow = qMin(maxRow,height - i);
        if(maxRow <= 1)
        {
            break;
        }
        while(j < width)
        {
            int maxCol = mosaicSize;
            maxCol = qMin(maxCol,width - j);
            if(maxCol <= 1)
            {
                break;
            }

            quint64 sumColorR = 0;
            quint64 sumColorG = 0;
            quint64 sumColorB = 0;
            for(int row = 0; row < maxRow; ++row)
            {
                for(int col = 0; col < maxCol; col++)
                {
                    indexR = (i + row) * bytePerLine + (j + col) * 4 + 2;
                    indexG = (i + row) * bytePerLine + (j + col) * 4 + 1;
                    indexB = (i + row) * bytePerLine + (j + col) * 4 + 0;
                    if(indexR >= image.byteCount())
                    {
                        qDebug()<<QString("indexR=%1,row=%2,col=%3,i=%4,j=%5").arg(indexR).arg(row).arg(col).arg(i).arg(j);
                        return;
                    }
                    r=imageBits[indexR];
                    g=imageBits[indexG];
                    b=imageBits[indexB];

                    sumColorR+=r;
                    sumColorG+=g;
                    sumColorB+=b;
                }
            }
            int avgColorR = sumColorR / (maxRow * maxCol);
            int avgColorG = sumColorG / (maxRow * maxCol);
            int avgColorB = sumColorB / (maxRow * maxCol);
            for(int row = 0; row < maxRow; ++row)
            {
                for(int col = 0; col < maxCol; col++)
                {
                    indexR = (i + row) * bytePerLine + (j + col) * 4 + 2;
                    indexG = (i + row) * bytePerLine + (j + col) * 4 + 1;
                    indexB = (i + row) * bytePerLine + (j + col) * 4 + 0;
                    imageBits[indexR] = avgColorR;
                    imageBits[indexG] = avgColorG;
                    imageBits[indexB] = avgColorB;
                }
            }
            j += mosaicSize;
        }
        i += mosaicSize;
    }
    return;
}

void Mplib::MpStaticMethod::convertToGray(QImage &image, int percentageR, int percentageG, int percentageB)
{
    if(percentageR <= 0 || percentageG <= 0 || percentageB <=0)
    {
        return;
    }
    int sumRgb = percentageB + percentageG + percentageR;

    int width = image.width();
    int height = image.height();
    uchar* imageBits = image.bits();
    int bytePerLine=image.bytesPerLine();
    qDebug()<<QString("width=%1,height=%2,bytePerLine=%3").arg(width).arg(height).arg(bytePerLine)<<image.byteCount();
    int indexR=0;
    int indexG=0;
    int indexB=0;
    int r=0;
    int g=0;
    int b=0;

    for(int row = 0; row < height; ++row)
    {
        int lineS = bytePerLine * row;
        for(int col = 0; col < width; col++)
        {
            indexR = lineS + col * 4 + 2;
            indexG = lineS + col * 4 + 1;
            indexB = lineS + col * 4 + 0;
            if(indexR >= image.byteCount())
            {
                qDebug()<<QString("indexR=%1,row=%2,col=%3").arg(indexR).arg(row).arg(col);
                return;
            }
            r=imageBits[indexR];
            g=imageBits[indexG];
            b=imageBits[indexB];
            int rgb = (r * percentageR + g * percentageG + b * percentageB) / sumRgb;

            imageBits[indexR] = rgb;
            imageBits[indexG] = rgb;
            imageBits[indexB] = rgb;

        }
    }
}

//http://www.ruanyifeng.com/blog/2012/11/gaussian_blur.html
void Mplib::MpStaticMethod::convertToGaussianBlur(QImage &image, int gaussianRadius, qreal deta)
{
    if(gaussianRadius < 1 || deta < 0.0001)
    {
        return;
    }
    qDebug()<<QString("image.format=%1").arg(image.format());

    int width = image.width();
    int height = image.height();
    QImage tempImage = image;
    uchar* tempImageBits = tempImage.bits();
    uchar* imageBits = image.bits();
    int radius = 2 * gaussianRadius + 1;
    qreal** gaussianRect = new qreal*[radius];
    for(int i =0; i < radius;++i)
    {
        gaussianRect[i] = new qreal[radius];
    }
    qreal deta2DaoShu = 0.5 / (deta * deta);
    qreal rate = deta2DaoShu / M_PI;

    qreal sum = 0.0;
    for(int i = 0; i < radius;++i)
    {
        for(int j = 0; j < radius;++j)
        {
            int x = i - radius / 2;
            int y = j - radius / 2;
            qreal v = rate * qExp(-1.0 * (x*x + y*y) * deta2DaoShu);
            gaussianRect[i][j]= v;
            sum += v;
        }
    }
    for(int i = 0; i < radius;++i)
    {
        for(int j = 0; j < radius;++j)
        {
            gaussianRect[i][j] /= sum;
        }
    }

    int bytePerLine=image.bytesPerLine();
    qDebug()<<QString("width=%1,height=%2,bytePerLine=%3").arg(width).arg(height).arg(bytePerLine)<<image.byteCount();
    int indexR=0;

    for(int row = 0; row < height; row++)
    {
        for(int col = 0; col < width;col++)
        {
            qreal sumR = 0.0;
            qreal sumG = 0.0;
            qreal sumB = 0.0;
            for(int i = 0; i < radius;++i)
            {
                for(int j = 0; j < radius;++j)
                {
                    int x = i - radius / 2 + col;
                    int y = j - radius / 2 + row;
                    if(x < 0)
                    {
                        x = -x;
                    }
                    else if(x > width - 1)
                    {
                        x = 2 * (width -1) - x;
                    }
                    if(x < 0 || x > width - 1)
                    {
                        qWarning()<<QString("width=%1,height=%2,bytePerLine=%3,x=%4 is out").arg(width).arg(height).arg(bytePerLine).arg(x);
                        break;
                    }
                    if(y < 0)
                    {
                        y = -y;
                    }
                    else if(y > height - 1)
                    {
                        y = 2 * (height -1) - y;
                    }
                    if(y < 0 || y > height - 1)
                    {
                        qWarning()<<QString("width=%1,height=%2,bytePerLine=%3,y=%4 is out").arg(width).arg(height).arg(bytePerLine).arg(y);
                        break;
                    }
                    indexR = y * bytePerLine + x * 4 + 2;
                    sumR += gaussianRect[j][i] * tempImageBits[indexR];
                    sumG += gaussianRect[j][i] * tempImageBits[indexR - 1];
                    sumB += gaussianRect[j][i] * tempImageBits[indexR - 2];
                }
            }
            indexR = row * bytePerLine + col * 4 + 2;
            imageBits[indexR] = sumR ;
            imageBits[indexR - 1] = sumG ;
            imageBits[indexR - 2] = sumB ;
        }
    }

    for(int i = 0; i < radius;++i)
    {
        delete[]gaussianRect[i];
    }
    delete []gaussianRect;
    return;
}
