#ifndef _CSCREENPUBLIC_H_
#define _CSCREENPUBLIC_H_

#include <QRect>
#include <QList>
#include "cscreenshot_global.h"

#define C_SCREEN_ZERO  0.000000001

enum CScreenShotStatus
{
    CSCREEN_SHOT_STATE_INITIALIZED,
    CSCREEN_SHOT_STATE_LOCKED,
    CSCREEN_SHOT_STATE_SELECTED,
    CSCREEN_SHOT_STATE_EDITED,
    CSCREEN_SHOT_STATE_FINISHED,
    CSCREEN_SHOT_STATE_CANCEL
};

enum CScreenPositionType
{
    CSCREEN_POSITION_TYPE_NOT_CONTAIN,
    CSCREEN_POSITION_TYPE_CONTAIN,
    CSCREEN_POSITION_TYPE_TOP_LEFT,
    CSCREEN_POSITION_TYPE_TOP_MIDDLE,
    CSCREEN_POSITION_TYPE_TOP_RIGHT,
    CSCREEN_POSITION_TYPE_LEFT_MIDDLE,
    CSCREEN_POSITION_TYPE_RIGHT_MIDDLE,
    CSCREEN_POSITION_TYPE_BOTTOM_LEFT,
    CSCREEN_POSITION_TYPE_BOTTOM_MIDDLE,
    CSCREEN_POSITION_TYPE_BOTTOM_RIGHT
};

enum CScreenButtonType
{
    CSCREEN_BUTTON_TYPE_UNDEFINED,
    CSCREEN_BUTTON_TYPE_RECT,
    CSCREEN_BUTTON_TYPE_ELLIPSE,
    CSCREEN_BUTTON_TYPE_ARROW,
    CSCREEN_BUTTON_TYPE_CANCLE,
    CSCREEN_BUTTON_TYPE_OK,
};

enum CScreenArrowDirection
{
    CSCREEN_ARROW_DIRECTION_UP,
    CSCREEN_ARROW_DIRECTION_DOWN,
};

// 获取窗口矩形列表
extern QList<QRect> CSCREENSHOTSHARED_EXPORT getWindownRectList();

#endif // _CSCREENPUBLIC_H_

