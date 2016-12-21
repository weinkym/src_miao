#ifndef CWHITEBOARDEDITORBUTTON_H
#define CWHITEBOARDEDITORBUTTON_H

#include <QWidget>
#include <QPushButton>

class CWhiteBoardEditorButton : public QWidget
{
    Q_OBJECT

public:
    CWhiteBoardEditorButton(const QString &normalIamgeUrl,const QString &hoverIamgeUrl,const QString &pressedIamgeUrl,int type,QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CWhiteBoardEditorButton();
    void setSelected(bool selected);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void sigClicked();

private:
    bool m_isPressed;
    bool m_selected;
    QString m_normalIamgeUrl;
    QString m_hoverIamgeUrl;
    QString m_pressedIamgeUrl;
};

#endif // CWHITEBOARDEDITORBUTTON_H
