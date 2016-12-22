#include "cwhiteboardview.h"
#include <QResizeEvent>
#include "cwhiteboarditem.h"
#include "cwhiteboardscene.h"

CWhiteBoardView::CWhiteBoardView(QWidget *parent)
    :QGraphicsView(parent)
{
    this->setStyleSheet(QString("QGraphicsView{border: 0px solid #000000;}"));
    m_whiteBoardItem = new  CWhiteBoardItem;
    m_scene = new CWhiteBoardScene;
    m_scene->addItem(m_whiteBoardItem);
    this->setScene(m_scene);
}

CWhiteBoardView::~CWhiteBoardView()
{

}

void CWhiteBoardView::clear()
{
    m_whiteBoardItem->clear();
}

void CWhiteBoardView::setDrawType(CWB::DrawType type)
{
    m_whiteBoardItem->setDrawType(type);
}

void CWhiteBoardView::setLineColor(const QColor &color)
{
    m_whiteBoardItem->setLineColor(color);
}

void CWhiteBoardView::setLineWidth(int width)
{
    m_whiteBoardItem->setLineWidth(width);
}

void CWhiteBoardView::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
//    size /= 2;
    m_scene->setSceneRect(QRect(0,0,size.width(),size.height()));
    QPixmap pixmap(size);
    pixmap.fill(QColor("#912a09"));
    m_whiteBoardItem->setPixmap(pixmap);
    return QGraphicsView::resizeEvent(event);
}
