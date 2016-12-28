#ifndef CWHITEBOARDEDITORBAR_H
#define CWHITEBOARDEDITORBAR_H

#include <QWidget>
#include "cwhiteboardeditorbutton.h"
#include "cwhiteboard_global.h"

class QHBoxLayout;
class CWhiteBoardView;
class CWHITEBOARDSHARED_EXPORT CWhiteBoardEditorBar : public QWidget
{
    Q_OBJECT

public:
    CWhiteBoardEditorBar(CWhiteBoardView *whiteBoardView,QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void setBackgroundColor(const QColor &color);
    int lineWidth() const {return m_lineWidth;};
    QColor lineColor() const {return m_color;};

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initButtons();
    void addButton(const QString &normalIamgeUrl,const QString &hoverIamgeUrl,const QString &pressedIamgeUrl,CWhiteBoardEditorButton::EditButtonType type);

private slots:
    void onButtonClicked();

signals:
    void sigClear();
    void sigDrawTypeChanged(CWB::DrawType type);
    void sigLineColorChanged(const QColor &color);
    void sigLineWidthChanged(int width);
    void sigUndo();
    void sigRedo();

private:
    QHBoxLayout *m_hLayout;
    QColor m_color;
    int m_lineWidth;
    QList<CWhiteBoardEditorButton*> m_buttons;
};

#endif // CWHITEBOARDEDITORBAR_H
