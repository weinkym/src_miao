#include "zwgui.h"
#include "zwlog.h"

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
