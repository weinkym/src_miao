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
}

#endif // ZWGUI_H
