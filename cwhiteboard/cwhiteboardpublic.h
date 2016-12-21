#ifndef CWHITEBOARDPUBLIC_H
#define CWHITEBOARDPUBLIC_H
#include <QDebug>
#include <QColor>
namespace CWB
{
enum DrawType{
    DRAW_TYPE_UNDEFINE,
    DRAW_TYPE_RECT,
    DRAW_TYPE_ELLIPSE,
    DRAW_TYPE_PEN,
    DRAW_TYPE_ARROW,
};
struct DrawParam
{
    DrawParam():type(DRAW_TYPE_RECT),lineColor(QColor(Qt::black)),width(0){};
    DrawType type;
    QColor lineColor;
    int width;
};

enum EditButtonType
{
    EDIT_TYPE_UNDEFINE,
    EDIT_TYPE_RECT,
    EDIT_TYPE_ELLIPSE,
    EDIT_TYPE_ARROW,
    EDIT_TYPE_CANCLE,
    EDIT_TYPE_OK,
};

enum CWhiteBoardArrowDirection
{
    ARROW_DIRECTION_UP,
    ARROW_DIRECTION_DOWN,
};

}

#define C_WhiteBoard_ZERO 0.000000001
#define C_WhiteBoardSHOT_LOG_INFO(x)
#endif // CWHITEBOARDPUBLIC_H
