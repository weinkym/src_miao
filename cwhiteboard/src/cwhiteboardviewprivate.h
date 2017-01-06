#ifndef CWHITEBOARDVIEWPRIVATE_H
#define CWHITEBOARDVIEWPRIVATE_H
#include <QColor>
#include <QRectF>

class CWhiteBoardItem;
class CWhiteBoardScene;
#include "cwhiteboard_global.h"

class CWHITEBOARDSHARED_EXPORT CWhiteBoardViewPrivate
{
public:
    CWhiteBoardViewPrivate();

private:
    friend class CWhiteBoardView;
    bool m_needScale;
    QRectF m_baseRect;
    QColor m_backgroundColor;
    CWhiteBoardItem *m_whiteBoardItem;
    CWhiteBoardScene *m_scene;
};

#endif // CWHITEBOARDVIEWPRIVATE_H
