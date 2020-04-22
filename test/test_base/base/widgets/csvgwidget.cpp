#include "csvgwidget.h"
#include <QPainter>
#include <QSvgRenderer>

CSvgWidget::~CSvgWidget()
{
}

void CSvgWidget::setSvgFile(const QString &filePath)
{
    if(m_svgRenderer == NULL)
    {
        m_svgRenderer = new QSvgRenderer(this);
        connect(m_svgRenderer, SIGNAL(repaintNeeded()), this, SLOT(onRepaintNeeded()));
    }
    m_svgRenderer->load(filePath);
}

CSvgWidget::CSvgWidget(QWidget *parent, Qt::WindowFlags f)
    : CClickWidget(parent, f)
    , m_svgRenderer(NULL)
{
}

void CSvgWidget::paintEvent(QPaintEvent *event)
{
    if(m_svgRenderer && m_svgRenderer->isValid())
    {
        QPainter painter(this);
        m_svgRenderer->render(&painter, QRectF(QPointF(0, 0), this->size()));
    }
    else
    {
        CClickWidget::paintEvent(event);
    }
}

void CSvgWidget::onRepaintNeeded()
{
    this->update();
}
