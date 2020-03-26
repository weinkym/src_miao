#ifndef CWHITEBOARDEDITORBAR_H
#define CWHITEBOARDEDITORBAR_H

#include <QWidget>
#include "cwhiteboard_global.h"

class CWhiteBoardEditorBarPrivate;
class CWhiteBoardView;

class CWHITEBOARDSHARED_EXPORT CWhiteBoardEditorBar : public QWidget
{
    Q_OBJECT

public:
    CWhiteBoardEditorBar(CWhiteBoardView *whiteBoardView,QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CWhiteBoardEditorBar();
    void setBackgroundColor(const QColor &color);
    int lineWidth() const;
    QColor lineColor() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    CWhiteBoardEditorBarPrivate *m_data;
};

#endif // CWHITEBOARDEDITORBAR_H
