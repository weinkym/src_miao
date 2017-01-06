#include "cwhiteboardview.h"
#include <QResizeEvent>
#include <QApplication>
#include <QScrollBar>
#include "cwhiteboarditem.h"
#include "cwhiteboardscene.h"
#include "cwhiteboardviewprivate.h"

CWhiteBoardView::CWhiteBoardView(QWidget *parent)
    :QGraphicsView(parent)
    ,m_data(new CWhiteBoardViewPrivate)
{
    this->setStyleSheet(QString("QGraphicsView{border: 0px solid #000000;background: transparent;}"));
//    this->setStyleSheet(QString("QGraphicsView{border: 0px solid #000000;background: #00ff00;}"));
    this->setScene(m_data->m_scene);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CWhiteBoardView::~CWhiteBoardView()
{
    delete m_data;
    m_data = NULL;
}

void CWhiteBoardView::setEditable(bool enabled)
{
    m_data->m_whiteBoardItem->setEditable(enabled);
}

void CWhiteBoardView::setBackgroundColor(const QColor &color)
{
    if(color == m_data->m_backgroundColor)
    {
        return;
    }
    m_data->m_backgroundColor = color;
    updateSize(this->size());
}

void CWhiteBoardView::setExpanding(bool enabled)
{
    if(m_data->m_needScale == enabled)
    {
        return;
    }
    m_data->m_needScale = enabled;
    if(m_data->m_needScale)
    {
        m_data->m_baseRect = m_data->m_scene->sceneRect();
    }
    updateSize(this->size());
}


void CWhiteBoardView::onClear()
{
    m_data->m_whiteBoardItem->clear();
}

void CWhiteBoardView::onDrawTypeChanged(CWB::DrawType type)
{
    m_data->m_whiteBoardItem->setDrawType(type);
}

void CWhiteBoardView::onLineColorChanged(const QColor &color)
{
    m_data->m_whiteBoardItem->setLineColor(color);
}

void CWhiteBoardView::onLineWidthChanged(int width)
{
    m_data->m_whiteBoardItem->setLineWidth(width);
}

void CWhiteBoardView::onUndo()
{
    m_data->m_whiteBoardItem->undo();
}

void CWhiteBoardView::onRedo()
{
    m_data->m_whiteBoardItem->redo();
}

void CWhiteBoardView::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    updateSize(size);
    return QGraphicsView::resizeEvent(event);
}

void CWhiteBoardView::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit sigDoubleClicked();
    QGraphicsView::mouseDoubleClickEvent(e);
}

void CWhiteBoardView::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit sigClicked();
    }
    QGraphicsView::mousePressEvent(e);
}

void CWhiteBoardView::updateSize(const QSize &size)
{
    if(size.width() <= 0 && size.height() <= 0)
    {
        return;
    }
    QSize adjustSize = size;
    if(m_data->m_needScale)
    {
        QRectF sceneRect = m_data->m_baseRect;
        if(sceneRect.width() < 0.001 && sceneRect.height() < 0.001)
        {
            return;
        }

        qreal sx = size.width() /  sceneRect.width();
        qreal sy = size.height() /  sceneRect.height();
        QTransform form = this->transform();
        form.setMatrix(sx,form.m12(),form.m13(),form.m21(),sy,form.m23(),form.m31(),form.m32(),form.m33());
        this->setTransform(form);
        adjustSize = sceneRect.size().toSize();
    }
    else
    {
        QTransform form = this->transform();
        form.reset();
        this->setTransform(form);

        m_data->m_scene->setSceneRect(QRect(0,0,adjustSize.width(),adjustSize.height()));
    }
    if(m_data->m_backgroundColor.alpha() == 0)
    {
        m_data->m_backgroundColor.setAlpha(1);
    }
    QPixmap pixmap(adjustSize);
    pixmap.fill(m_data->m_backgroundColor);
    m_data->m_whiteBoardItem->setPixmap(pixmap);
}
