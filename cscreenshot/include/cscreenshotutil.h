#ifndef _CSCREENSHOTUTIL_H_
#define _CSCREENSHOTUTIL_H_

#include <QRectF>
#include <QLineF>
#include "cscreenpublic.h"

class CScreenshotUtil
{
public:
    static QRectF convertLineFToRectF(const QLineF &line);
    static QRect convertLineToRect(const QLine &line);
    static bool isEditType(CScreenButtonType type);
};

#endif // _CSCREENSHOTUTIL_H_
