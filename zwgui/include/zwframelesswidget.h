#ifndef ZWFRAMELESSWIDGET_H
#define ZWFRAMELESSWIDGET_H

//参考https://zhuanlan.zhihu.com/p/89589678
//Qt源码分享（一）-- 圆角+阴影+可移动+大小可变

#include <QWidget>

class ZWFramelessWidget : public QWidget
{
    Q_OBJECT

enum Direction{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    LEFTTOP,
    LEFTBOTTOM,
    RIGHTBOTTOM,
    RIGHTTOP,
    NONE
};

public:
    explicit ZWFramelessWidget(QWidget *parent = nullptr);
    ~ZWFramelessWidget();

public:
    void SetResizeable(bool isResize);
    bool IsResizeable() const;

    void SetRadius(bool radius);
    int GetRadius() const;

protected:
    void resizeEvent(QResizeEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void region(const QPoint &cursorPoint);

    /*!
     * \brief 绘制带尖的图片
     * \param w      图片宽度
     * \param h      图片高度
     * \param tip_w  尖的宽度
     * \param tip_h  尖的高度
     * \param radius 矩形圆角半径
     * \param margin 边距
     * \return
     */
    QPixmap drawTip(const int w, const int h, const int tip_w, const int tip_h,
                    const int radius, const int margin);

private:
    QPixmap     m_pixmap;
    bool        mIsLeftPressDown;
    QPoint      mDragPosition;
    Direction   mDirec;
    bool        mResizeable;
    int         mRadius;

};

#endif // ZWFRAMELESSWIDGET_H
