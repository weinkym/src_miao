#ifndef CWHITEBOARDVIEW_H
#define CWHITEBOARDVIEW_H
#include <QGraphicsView>
#include "cwhiteboardpublic.h"
#include "cwhiteboard_global.h"

class CWhiteBoardViewPrivate;
class CWHITEBOARDSHARED_EXPORT CWhiteBoardView : public QGraphicsView
{
    Q_OBJECT
public:
    CWhiteBoardView(QWidget *parent = Q_NULLPTR);
    ~CWhiteBoardView();
    void setEditable(bool enabled);
    void setBackgroundColor(const QColor &color);

protected:
    void resizeEvent(QResizeEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    void updateSize(const QSize &size);

public slots:
    void onClear();
    void onDrawTypeChanged(CWB::DrawType type);
    void onLineColorChanged(const QColor &color);
    void onLineWidthChanged(int width);
    void onUndo();
    void onRedo();

signals:
    void sigDoubleClicked();
    void sigClicked();

private:
    CWhiteBoardViewPrivate *m_data;
};

#endif // CWHITEBOARDVIEW_H
