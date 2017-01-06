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
    QSize adjustSize = size;
    QTransform transform = this->transform();
    qreal m11 = transform.m11();
    qreal m22 = transform.m22();

    if(m11 > 0.001 && m22 > 0.001)
    {
        adjustSize.setWidth(size.width() / m11);
        adjustSize.setHeight(size.height() / m22);
    }
    m_data->m_scene->setSceneRect(QRect(0,0,adjustSize.width(),adjustSize.height()));
    if(m_data->m_backgroundColor.alpha() == 0)
    {
        m_data->m_backgroundColor.setAlpha(1);
    }
    QPixmap pixmap(adjustSize);
    pixmap.fill(m_data->m_backgroundColor);
    m_data->m_whiteBoardItem->setPixmap(pixmap);
}
