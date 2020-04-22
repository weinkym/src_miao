#include "ctestwidget.h"
#include "clogsetting.h"
#include <QPainter>
#include <QSvgRenderer>

CTestWidget::~CTestWidget()
{
}

CTestWidget::CTestWidget(QWidget *parent, Qt::WindowFlags f)
    : CBaseWidget(parent, f)
    , m_svgRenderer(new QSvgRenderer(QString(":/res/test.svg")))
{
    connect(m_svgRenderer, SIGNAL(repaintNeeded()), this, SLOT(onRepaintNeeded()));
}

void CTestWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    m_svgRenderer->render(&painter, QRectF(QPointF(0, 0), this->size()));
}

void CTestWidget::onRepaintNeeded()
{
    C_LOG_FUNCTION;
    this->update();
}
