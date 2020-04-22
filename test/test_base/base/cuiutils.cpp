#include "cuiutils.h"
#include "clogsetting.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QSvgRenderer>

CUIUtils::CUIUtils()
{
}

CUIUtils::~CUIUtils()
{
}

QStringList CUIUtils::elidedTextList(const QString &text, const QFont &font, int width, uint maxRow, const QString &fix)
{
    QFontMetrics fontMetrics(font);
    QSize textSize = fontMetrics.tightBoundingRect(text).size();
    if(textSize.width() <= width)
    {
        return QStringList(text);
    }
    textSize = fontMetrics.tightBoundingRect(fix).size();
    if(textSize.width() > width)
    {
        return QStringList(fix);
    }
    uint row = 1;
    QStringList stringList;
    QString lineString;
    bool isOut = false;
    QString preLineString;
    foreach(QString c, text)
    {
        if(isOut)
        {
            if(row >= maxRow)
            {
                stringList.append(preLineString + fix);
                preLineString = "";
                lineString = "";
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
                    preLineString = "";
                    lineString = "";
                }
            }
            else
            {
                stringList.append(preLineString);
                row++;
                isOut = false;
                preLineString = "";
                lineString = lineString.right(1);
            }
        }

        QString tempText = lineString + c + fix;
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

QStringList CUIUtils::elidedTextListWidthlineBreak(const QString &source, const QFont &font, int width, uint maxRow, const QString &subffix)
{
    QStringList resultList;
    QStringList contentList = source.split("\n");
    uint currentRow = 0;
    foreach(QString text, contentList)
    {
        QStringList tempList = CUIUtils::elidedTextList(text, font, width, maxRow - currentRow, subffix);
        foreach(QString lineString, tempList)
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

void CUIUtils::setFullChildWidget(QWidget *parent, QWidget *childWidget, const QMargins &margins)
{
    if(parent && childWidget)
    {
        QHBoxLayout *layout = new QHBoxLayout(parent);
        layout->setSpacing(0);
        layout->setContentsMargins(margins);
        layout->addWidget(childWidget);
        parent->setLayout(layout);
    }
}

QPixmap CUIUtils::getSvgFilePixmap(const QString &filePath, const QSize &size, int devicePixelRatio)
{
    if(devicePixelRatio != 2)
    {
        devicePixelRatio = 1;
    }

    QSvgRenderer svgRender(filePath);
    if(svgRender.isValid())
    {
        QSize baseSize = (size.isValid() && size.width() > 0 && size.height() > 0) ? size : svgRender.defaultSize();
        C_LOG_OUT_V3(baseSize, size, svgRender.defaultSize());
        QPixmap pixmap(baseSize * devicePixelRatio);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        svgRender.render(&painter, QRectF(0, 0, pixmap.width(), pixmap.height()));
        pixmap.setDevicePixelRatio(devicePixelRatio);
        return pixmap;
    }
    else
    {
        C_LOG_WARNING(QString("load svg file is error,fp=%1").arg(filePath));
    }
    return QPixmap();
}
