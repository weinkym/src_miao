#ifndef ZSCREENSHOT_H
#define ZSCREENSHOT_H

#include <QWidget>
#include <QDialog>
#include <QStack>
#include <QLineEdit>
#include <QButtonGroup>
#include "mplib_global.h"
class QComboBox;
class QPushButton;
class ZScreenEditor;

class MPLIBSHARED_EXPORT ZScreenshotDlg : public QDialog
{
    Q_OBJECT

public:
    ZScreenshotDlg(QWidget* parent = 0);
    QPixmap getSelectedImg();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void initSelectParam();
    void drawWindow();

private slots:
    void onSigReset();
    void onSigSelectedImg(const QPixmap& pix);
    void onSigCancel();

//signals:
//    void sigSelectImg(const QPixmap& pix);

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    bool m_bGrabing;
    QImage m_bgImg;
    ZScreenEditor* m_editor;
    QPixmap m_selectImg;
};


//================================================================
class QToolButton;
class QToolBar;
#define BTN_DEFAULT_SIZE 50

class MPLIBSHARED_EXPORT ZScreenEditor : public QWidget
{
    Q_OBJECT
public:
    enum Option{NoOption,DrawFree,DrawCircle,DrawRect,DrawText,UNDO,SAVEAS,CANCEL,OK};
    ZScreenEditor(const QPixmap& img,QWidget* parent = 0);
    void resetByImg(const QPixmap& img);
    QPixmap getSelectedImg();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
//    void moveEvent(QMoveEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    void updateForImg(const QPixmap& img);
    void updateToolBar();
    void initToolBar();
    void paint();
    bool save(const QString& path);

    void addButton(const QIcon& icon,const QString& text,QButtonGroup* group,int id);

private slots:
    void undo();
    void saveAs();
    void onCancelBtnClicked();
    void onCompleteBtnClicked();

    void onBtnClicked(int id);

signals:
    void sigReset();
    void sigSelectImg(const QPixmap& pix);
    void sigCancel();

private:
    QToolBar* toolBar;
    Option m_curOption;
    QPixmap m_image;
    QImage m_tempImg;
    QImage m_bufferImg;
    QStack<QImage> m_stackImages;
    bool m_bDrawing;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    QPoint m_curPos;
    QPoint m_oldPos;
    //=======================
    QLineEdit* m_lineEdit;
};

#endif // ZSCREENSHOT_H
