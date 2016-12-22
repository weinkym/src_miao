#ifndef CWHITEBOARDWIDGET_H
#define CWHITEBOARDWIDGET_H

#include <QWidget>

namespace Ui {
class CWhiteBoardWidget;
}

class CWhiteBoardEditorButton;
class CWhiteBoardView;

class CWhiteBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CWhiteBoardWidget(QWidget *parent = 0);
    ~CWhiteBoardWidget();

private:
    void initButtons();
    void addButton(CWhiteBoardEditorButton *button);

private slots:
    void onButtonClicked();

private:
    Ui::CWhiteBoardWidget *ui;
    CWhiteBoardView *m_whiteBoardView;
    QColor m_color;
    int m_lineWidth;
    QList<CWhiteBoardEditorButton*> m_buttons;
};

#endif // CWHITEBOARDWIDGET_H
