#include "cscreenshotutil.h"

QRectF CScreenshotUtil::convertLineFToRectF(const QLineF &line)
{
    QPointF topLeft(qMin(line.x1(),line.x2()),qMin(line.y1(),line.y2()));
    QPointF bottomRight(qMax(line.x1(),line.x2()),qMax(line.y1(),line.y2()));
    return QRectF(topLeft,bottomRight);
}

QRect CScreenshotUtil::convertLineToRect(const QLine &line)
{
    QPoint topLeft(qMin(line.x1(),line.x2()),qMin(line.y1(),line.y2()));
    QPoint bottomRight(qMax(line.x1(),line.x2()),qMax(line.y1(),line.y2()));
    return QRect(topLeft,bottomRight);
}

bool CScreenshotUtil::isEditType(CScreenButtonType type)
{
    return (type == CSCREEN_BUTTON_TYPE_RECT || type == CSCREEN_BUTTON_TYPE_ELLIPSE || type == CSCREEN_BUTTON_TYPE_ARROW);
}
