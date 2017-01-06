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
    DRAW_TYPE_ERASER,
    DRAW_TYPE_TEXT,
    DRAW_TYPE_LINE,
    DRAW_TYPE_POINT,
};
struct DrawParam
{
    DrawParam():type(DRAW_TYPE_RECT),lineColor(QColor(Qt::black)),width(0){};
    DrawType type;
    QColor lineColor;
    int width;
    QVariant value;
};

enum CWhiteBoardArrowDirection
{
    ARROW_DIRECTION_UP,
    ARROW_DIRECTION_DOWN,
};

}

#endif // CWHITEBOARDPUBLIC_H
