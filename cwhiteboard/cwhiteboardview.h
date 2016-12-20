#ifndef CWHITEBOARDVIEW_H
#define CWHITEBOARDVIEW_H
#include <QGraphicsView>

class CWhiteBoardItem;
class CWhiteBoardScene;
class CWhiteBoardView : public QGraphicsView
{
public:
    CWhiteBoardView(QWidget *parent = Q_NULLPTR);
    ~CWhiteBoardView();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    CWhiteBoardItem *m_whiteBoardItem;
    CWhiteBoardScene *m_scene;
};

#endif // CWHITEBOARDVIEW_H
