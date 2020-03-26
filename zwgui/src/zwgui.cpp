#include "zwgui.h"
#include "zwlog.h"
#include <QtMath>
#include <QCursor>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

bool ZWGui::printQrcode(const QImage &image, bool removeMagin,
                        int hasValueColor, int noValueColor, zw_qrcode_color_judge_fun fun)
{
    if(hasValueColor > 7 || hasValueColor < 0)
    {
        hasValueColor = 0;
    }
    if(noValueColor > 7 || noValueColor < 0)
    {
        noValueColor = 7;
    }
    zw_qrcode_color_judge_fun runFun = nullptr;
    if(fun == nullptr)
    {
       runFun = [](const QColor &color){
           return color.red() > 200;
       } ;
    }
    else
    {
        runFun = fun;
    }
    if(image.isNull())
    {
        ZW_LOG_WARNING(QString("image is null"));
        return false;
    }
    if(image.width() < 25 || image.height() < 25)
    {
        ZW_LOG_WARNING(QString("image size is invalid ,size >(25 * 25) w=%1,h=%2").arg(image.width()).arg(image.height()));
        return false;
    }
    int aduestX = image.width();
    bool hasValue = (runFun(image.pixelColor(0,0)));
    int currentDx = 1;

    int valueLeft = image.width();
    int valueRight = 0;
    int valueBottom = 0;
    int valueTop = 0;

    for(int i = 0; i <image.width();++i)
    {
        for(int j = 0; j < image.height();++j)
        {
            if(runFun(image.pixelColor(i,j)))
            {
                if(hasValue)
                {
                    currentDx = 1;
                }
                else
                {
                    valueRight = qMax(j-1,valueRight);
                    aduestX = qMin(aduestX,currentDx);
                    currentDx = 1;
                    hasValue = true;
                }
            }
            else
            {
                valueBottom = i;
                if(hasValue)
                {
                    if(valueLeft > j)
                    {
                        valueLeft = j;
                        valueTop = i;
                    }
                    currentDx = 1;
                    hasValue = false;
                }
                else
                {
                    currentDx++;
                }
            }
        }
    }
    hasValue = (image.pixelColor(0,0).red() > 200);

    int startX = removeMagin ? valueLeft : 0;
    int startY = removeMagin ? valueTop : 0;
    int i = startX;
    int j = startY;
    int maxX = removeMagin ? (valueRight+1):image.width();
    int maxY = removeMagin ? (valueBottom+1):image.height();
    printf("\n");
    printf("\e[0m");
    while(j < maxY)
    {
        i = startX;
        while(i < maxX)
        {
            if(image.pixelColor(i,j).red() > 200)
            {
                printf("\e[0;%dm  ",40 + noValueColor);
//                printf("\e[0m  ");
            }
            else
            {
                printf("\e[0;%dm  ",40 + hasValueColor);
//                printf("\e[7m  ");
            }
            i += aduestX;
        }
        j += aduestX;
        printf("\e[0m");
        printf("\n");
    }
    printf("\e[0m");
    //    显示：0(默认)、1(粗体/高亮)、22(非粗体)、4(单条下划线)、24(无下划线)、5(闪烁)、25(无闪烁)、7(反显、翻转前景色和背景色)、27(无反显)
    //        颜色：0(黑)、1(红)、2(绿)、 3(黄)、4(蓝)、5(洋红)、6(青)、7(白)
    //        前景色为30+颜色值，如31表示前景色为红色；背景色为40+颜色值，如41表示背景色为红色。
    // \033[显示方式;前景色;背景色m输出字符串\033[0m 或 \e[显示方式;前景色;背景色m输出字符串\033[0m
    //TTTV:aduestX=4,startLeft=16,startRight=115,startTop=16,startBttom=115
    printf("TTTV:aduestX=%d,startLeft=%d,startRight=%d,startTop=%d,startBttom=%d\n",aduestX,valueLeft,valueRight,valueTop,valueBottom);;
}


void ZWGui::convertToMosaic(int mosaicSize, QImage &image)
{
    qDebug()<<QString("image.format=%1").arg(image.format());
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

void ZWGui::convertToGray(QImage &image, int percentageR, int percentageG, int percentageB)
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
void ZWGui::convertToGaussianBlur(QImage &image, int gaussianRadius, qreal deta)
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

QColor ZWGui::getCursorPosColor()
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
            }
        }
    }
    return color;
}

QPainterPath ZWGui::createPath(const QList<QPointF> &points)
{
    if(points.isEmpty())
    {
        return QPainterPath();
    }
    QPainterPath path(points[0]);
    for (int i = 0; i < points.size() - 1; ++i) {
        //Qt 绘制平滑曲线 http://www.qtdebug.com/qt-smooth-curve/
        // 控制点的 x 坐标为 sp 与 ep 的 x 坐标和的一半
        // 第一个控制点 c1 的 y 坐标为起始点 sp 的 y 坐标
        // 第二个控制点 c2 的 y 坐标为结束点 ep 的 y 坐标
        QPointF sp = points[i];
        QPointF ep = points[i+1];
        QPointF c1 = QPointF((sp.x() + ep.x()) / 2, sp.y());
        QPointF c2 = QPointF((sp.x() + ep.x()) / 2, ep.y());
        path.cubicTo(c1, c2, ep);
    }
    return path;
}

QStringList ZWGui::elidedTextList(const QString &text, const QFont &font, int width, uint maxRow, const QString &subffix)
{
    QFontMetrics fontMetrics(font);
    QSize textSize = fontMetrics.tightBoundingRect(text).size();
    if(textSize.width() <= width)
    {
        return QStringList(text);
    }
    textSize = fontMetrics.tightBoundingRect(subffix).size();
    if(textSize.width() > width)
    {
        return QStringList(subffix);
    }
    int row = 1;
    QStringList stringList;
    QString lineString;
    bool isOut = false;
    QString preLineString;
    foreach (QString c, text)
    {
        if(isOut)
        {
            if(row >= maxRow)
            {
                stringList.append(preLineString+subffix);
                preLineString="";
                lineString="";
                break;
            }

            QString tempText = lineString;
            textSize = fontMetrics.tightBoundingRect(tempText).size();
            if(textSize.width() <= width)
            {
                preLineString = lineString;
                lineString.append(c);
                QString tempText = lineString;
                textSize = fontMetrics.tightBoundingRect(tempText).size();
                if(textSize.width() <= width)
                {
                    preLineString = lineString;
                    continue;
                }
                else
                {
                    stringList.append(preLineString);
                    row++;
                    isOut = false;
                    preLineString="";
                    lineString = "";
                }
            }
            else
            {
                stringList.append(preLineString);
                row++;
                isOut = false;
                preLineString="";
                lineString = lineString.right(1);
            }
        }

        QString tempText = lineString + c + subffix;
        textSize = fontMetrics.tightBoundingRect(tempText).size();
        lineString.append(c);
        if(textSize.width() <= width)
        {
            preLineString = lineString;
        }
        else
        {
            isOut = true;
        }
    }
    if(!lineString.isEmpty() && row <= maxRow)
    {
        stringList.append(lineString);
    }

    return stringList;
}

QStringList ZWGui::elidedTextListWidthlineBreak(const QString &text, const QFont &font, int width, uint maxRow, const QString &subffix)
{
    QStringList resultList;
    QStringList contentList = text.split("\n");
    uint currentRow = 0;
    foreach (QString text, contentList)
    {
        QStringList tempList = elidedTextList(text,font,width,maxRow - currentRow,subffix);
        foreach (QString lineString, tempList)
        {
            if(currentRow < maxRow)
            {
                resultList.append(lineString);
                currentRow++;
            }
        }
        if(currentRow >= maxRow)
        {
            break;
        }
    }
    return resultList;
}
