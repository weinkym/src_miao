#include "zwframelesswidget.h"
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QResizeEvent>

static const int SHADOW_WIDTH = 16;

#define PADDING     18
#define PADDING_TOP 29

ZWFramelessWidget::ZWFramelessWidget(QWidget *parent) : QWidget(parent)
{
    mIsLeftPressDown    = false;
    this->mResizeable   = true;
    this->mDirec        = NONE;
    this->mRadius       = 4;
    this->setMouseTracking(true);

    QGraphicsDropShadowEffect* pShadowEffect = new QGraphicsDropShadowEffect(this);
    pShadowEffect->setOffset(0, 0);
    pShadowEffect->setColor(Qt::lightGray);
    pShadowEffect->setBlurRadius(SHADOW_WIDTH);
    this->setGraphicsEffect(pShadowEffect);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
}

ZWFramelessWidget::~ZWFramelessWidget()
{

}

void ZWFramelessWidget::SetResizeable(bool isResize)
{
    mResizeable = isResize;
}

bool ZWFramelessWidget::IsResizeable() const
{
    return mResizeable;
}

void ZWFramelessWidget::SetRadius(bool radius)
{
    mRadius = radius;
}

int ZWFramelessWidget::GetRadius() const
{
    return mRadius;
}

void ZWFramelessWidget::resizeEvent(QResizeEvent *e)
{
    const QSize s = e->size();
    int w = s.width();
    int h = s.height();
    if(w > 1 && h > 1) {
        m_pixmap = drawTip(w, h, 18, 12, mRadius, SHADOW_WIDTH);
    }
}

void ZWFramelessWidget::paintEvent(QPaintEvent *)
{
    if(m_pixmap.isNull())
        return;

    QPainter p;
    p.begin(this);
    p.drawPixmap(this->rect(), m_pixmap);
    p.end();
}

void ZWFramelessWidget::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING_TOP >= x && tl.x() <= x && tl.y() + PADDING_TOP >= y && tl.y() <= y) {
        // 左上角
        mDirec = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        mDirec = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        mDirec = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING_TOP && y >= tl.y() && y <= tl.y() + PADDING_TOP) {
        // 右上角
        mDirec = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        mDirec = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        mDirec = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING_TOP){
        // 上边
        mDirec = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
        mDirec = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        mDirec = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

QPixmap ZWFramelessWidget::drawTip(const int w, const int h, const int tip_w, const int tip_h, const int radius, const int margin)
{
    const int w2 = w - margin * 2 - radius * 2 - tip_w;

    QPainterPath path;
    //三角形
//    path.moveTo(radius + margin + w2 / 2, tip_h + margin);
//    path.lineTo(w / 2, margin);
//    path.lineTo(radius + margin + w2 / 2 + tip_w, tip_h + margin);

    //3个矩形
    path.addRect(margin+radius,margin+tip_h,w-(margin+radius)*2, h-margin*2-tip_h);
    path.addRect(margin,margin+tip_h+radius,radius,(h-margin-radius)-(margin+tip_h+radius));
    path.addRect(w-margin-radius,margin+tip_h+radius,radius,(h-margin-radius)-(margin+tip_h+radius));

    //4个圆角
    //
    path.moveTo(w-margin-radius, tip_h + margin + radius);
    path.arcTo(QRectF(margin + w2 + tip_w, tip_h + margin, radius * 2, radius * 2), 0, 90);
    //
    path.moveTo(w - margin-radius, h - margin - radius);
    path.arcTo(QRectF(margin + w2 + tip_w, h - margin - radius * 2, radius * 2, radius * 2), 270, 90);
    //
    path.moveTo(margin + radius, h -margin-radius);
    path.arcTo(QRectF(margin, h - margin - radius*2, radius * 2, radius * 2), 180, 90);
    //
    path.moveTo(margin + radius, margin + tip_h+radius);
    path.arcTo(QRectF(margin, margin+tip_h, radius * 2, radius * 2), 90, 90);

    //绘制到图片
    QPixmap pixmap(w, h);
    pixmap.fill(Qt::transparent);

    QPainter p;
    p.begin(&pixmap);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QBrush(QColor(249, 249, 249)));
    p.setPen(Qt::NoPen);
    p.drawPath(path);
    p.end();

    return pixmap;
}

void ZWFramelessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        mIsLeftPressDown = false;
        if(mDirec != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void ZWFramelessWidget::mousePressEvent(QMouseEvent *event)
{
    switch(event->button()) {
    case Qt::LeftButton:
        mIsLeftPressDown = true;
        if(mDirec != NONE) {
            this->mouseGrabber();
        } else {
            mDragPosition = event->globalPos() - this->frameGeometry().topLeft();
        }
        break;
    default:
        QWidget::mousePressEvent(event);
    }
}

void ZWFramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if(!mIsLeftPressDown) {
        if (mResizeable) {
            this->region(gloPoint);
        }
    } else {

        if(mDirec != NONE && mResizeable) {
            QRect rMove(tl, rb);

            switch(mDirec) {
            case LEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x() - SHADOW_WIDTH);
                else
                    rMove.setX(gloPoint.x() - SHADOW_WIDTH);
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x() + SHADOW_WIDTH);
                break;
            case UP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y() - PADDING_TOP);
                else
                    rMove.setY(gloPoint.y() - PADDING_TOP);
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y() + SHADOW_WIDTH);
                break;
            case LEFTTOP:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x() - PADDING);
                else
                    rMove.setX(gloPoint.x() - PADDING);
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y() - PADDING_TOP);
                else
                    rMove.setY(gloPoint.y() - PADDING_TOP);
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x() + PADDING);
                rMove.setY(gloPoint.y()-PADDING_TOP);
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x() - PADDING);
                rMove.setHeight(gloPoint.y() - tl.y() + PADDING);
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x() + PADDING);
                rMove.setHeight(gloPoint.y() - tl.y() + PADDING);
                break;
            default:
                break;
            }
            this->setGeometry(rMove);
        } else {
            move(event->globalPos() - mDragPosition);
            event->accept();
        }
    }
    QWidget::mouseMoveEvent(event);
}
