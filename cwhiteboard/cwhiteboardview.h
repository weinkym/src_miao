#ifndef CWHITEBOARDVIEW_H
#define CWHITEBOARDVIEW_H
#include <QGraphicsView>
#include "cwhiteboardpublic.h"

class CWhiteBoardItem;
class CWhiteBoardScene;
class CWhiteBoardView : public QGraphicsView
{
public:
    CWhiteBoardView(QWidget *parent = Q_NULLPTR);
    ~CWhiteBoardView();
    void clear();
    void setDrawType(CWB::DrawType type);
    void setLineColor(const QColor &color);
    void setLineWidth(int width);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    CWhiteBoardItem *m_whiteBoardItem;
    CWhiteBoardScene *m_scene;
};

#endif // CWHITEBOARDVIEW_H
