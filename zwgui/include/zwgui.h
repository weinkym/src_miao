#ifndef ZWGUI_H
#define ZWGUI_H
#include <QColor>
#include <QImage>
#include "zwgui_global.h"
#include <stdint.h>
#include <iostream>
#include <functional>

typedef std::function<bool(const QColor &color)> zw_qrcode_color_judge_fun;

namespace ZWGui
{
//        颜色：0(黑)、1(红)、2(绿)、 3(黄)、4(蓝)、5(洋红)、6(青)、7(白)
bool ZWGUISHARED_EXPORT printQrcode(const QImage &image, bool removeMagin = false, int hasValueColor = 0,int noValueColor = 7,zw_qrcode_color_judge_fun fun = nullptr);

QColor ZWGUISHARED_EXPORT getCursorPosColor();
void ZWGUISHARED_EXPORT convertToMosaic(int mosaicSize,QImage &image);
void ZWGUISHARED_EXPORT convertToGray(QImage &image,int percentageR,int percentageG,int percentageB);
void ZWGUISHARED_EXPORT convertToGaussianBlur(QImage &image,int gaussianRadius,qreal deta);

QPainterPath ZWGUISHARED_EXPORT createPath(const QList<QPointF> &points);


/*!
 * \brief 计算范围内文本长度,超过后采用后缀省略
 * \param source 源文本
 * \param font 字体信息
 * \param size 大小范围
 * \param subffix 后缀文本
 * \return
 */
QStringList ZWGUISHARED_EXPORT elidedTextList(const QString &text,const QFont &font,int width,uint maxRow,const QString &subffix = "...");
QStringList ZWGUISHARED_EXPORT elidedTextListWidthlineBreak(const QString &text,const QFont &font,int width,uint maxRow,const QString &subffix = "...");

}

#endif // ZWGUI_H
