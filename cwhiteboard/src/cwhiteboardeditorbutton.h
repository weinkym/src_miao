#ifndef CWHITEBOARDEDITORBUTTON_H
#define CWHITEBOARDEDITORBUTTON_H

#include <QWidget>
#include <QPushButton>
#include "cwhiteboardpublic.h"
#include "cwhiteboard_global.h"

class CWHITEBOARDSHARED_EXPORT CWhiteBoardEditorButton : public QWidget
{
    Q_OBJECT

public:
    enum EditButtonType
    {
        EDIT_TYPE_UNDEFINE,
        EDIT_TYPE_RECT,
        EDIT_TYPE_ELLIPSE,
        EDIT_TYPE_PEN,
        EDIT_TYPE_TEXT,
        EDIT_TYPE_ARROW,
        EDIT_TYPE_CANCLE,
        EDIT_TYPE_OK,
        EDIT_TYPE_ERASER,
        EDIT_TYPE_UNDO,
        EDIT_TYPE_REDO,
        EDIT_TYPE_CLEAR,
        EDIT_TYPE_COLOR,
        EDIT_TYPE_LINE_WIDTH,
        EDIT_TYPE_LINE,
        EDIT_TYPE_POINT,
    };
    CWhiteBoardEditorButton(const QString &normalIamgeUrl,const QString &hoverIamgeUrl,const QString &pressedIamgeUrl,CWhiteBoardEditorButton::EditButtonType type,QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CWhiteBoardEditorButton();
    void setSelected(bool selected);
    CWhiteBoardEditorButton::EditButtonType getType() const{return m_type;};

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void sigClicked();

private:
    bool m_isPressed;
    bool m_selected;
    CWhiteBoardEditorButton::EditButtonType m_type;
    QString m_normalIamgeUrl;
    QString m_hoverIamgeUrl;
    QString m_pressedIamgeUrl;
};

#endif // CWHITEBOARDEDITORBUTTON_H
