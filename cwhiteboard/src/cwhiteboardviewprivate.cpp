#include "cwhiteboardviewprivate.h"
#include "cwhiteboarditem.h"
#include "cwhiteboardscene.h"

CWhiteBoardViewPrivate::CWhiteBoardViewPrivate()
    :m_backgroundColor(QColor(255,255,255,1))
    ,m_whiteBoardItem(NULL)
    ,m_scene(NULL)
{
    m_whiteBoardItem = new  CWhiteBoardItem;
    m_scene = new CWhiteBoardScene;
    m_scene->addItem(m_whiteBoardItem);
}
