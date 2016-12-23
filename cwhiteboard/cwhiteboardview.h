#ifndef CWHITEBOARDVIEW_H
#define CWHITEBOARDVIEW_H
#include <QGraphicsView>
#include "cwhiteboardpublic.h"

class CWhiteBoardItem;
class CWhiteBoardScene;
class CWhiteBoardView : public QGraphicsView
{
    Q_OBJECT
public:
    CWhiteBoardView(QWidget *parent = Q_NULLPTR);
    ~CWhiteBoardView();

public slots:
    void onClear();
    void onDrawTypeChanged(CWB::DrawType type);
    void onLineColorChanged(const QColor &color);
    void onLineWidthChanged(int width);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    CWhiteBoardItem *m_whiteBoardItem;
    CWhiteBoardScene *m_scene;
};

#endif // CWHITEBOARDVIEW_H
