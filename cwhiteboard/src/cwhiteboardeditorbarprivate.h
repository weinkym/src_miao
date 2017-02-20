#ifndef CWHITEBOARDEDITORBARPRIVATE_H
#define CWHITEBOARDEDITORBARPRIVATE_H
#include <QGraphicsView>
#include "cwhiteboardeditorbutton.h"
#include "cwhiteboard_global.h"
#include "cwhiteboardpublic.h"

class QHBoxLayout;
class CWhiteBoardView;
class CWhiteBoardEditorBar;

class CWHITEBOARDSHARED_EXPORT CWhiteBoardEditorBarPrivate : QObject
{
    Q_OBJECT

public:
    CWhiteBoardEditorBarPrivate();

private:
    void initButtons();
    CWhiteBoardEditorButton* addButton(const QString &normalIamgeUrl,const QString &hoverIamgeUrl,const QString &pressedIamgeUrl,CWhiteBoardEditorButton::EditButtonType type);

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
    friend class CWhiteBoardEditorBar;
    QHBoxLayout *m_hLayout;
    QColor m_color;
    int m_lineWidth;
    QList<CWhiteBoardEditorButton*> m_buttons;
};

#endif // CWHITEBOARDEDITORBARPRIVATE_H
