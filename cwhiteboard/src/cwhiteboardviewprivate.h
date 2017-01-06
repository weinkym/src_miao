#ifndef CWHITEBOARDVIEWPRIVATE_H
#define CWHITEBOARDVIEWPRIVATE_H
#include <QColor>
class CWhiteBoardItem;
class CWhiteBoardScene;
#include "cwhiteboard_global.h"

class CWHITEBOARDSHARED_EXPORT CWhiteBoardViewPrivate
{
public:
    CWhiteBoardViewPrivate();

private:
    friend class CWhiteBoardView;
    QColor m_backgroundColor;
    CWhiteBoardItem *m_whiteBoardItem;
    CWhiteBoardScene *m_scene;
};

#endif // CWHITEBOARDVIEWPRIVATE_H
