#include "cwhiteboardeditorbar.h"
#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include "cwhiteboardview.h"
#include <QDebug>
#include "cwhiteboardeditorbarprivate.h"

CWhiteBoardEditorBar::CWhiteBoardEditorBar(CWhiteBoardView *whiteBoardView, QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent,f)
    ,m_data(new CWhiteBoardEditorBarPrivate)
{
    setBackgroundColor(QColor(62,65,70));
    this->setLayout(m_data->m_hLayout);
    if(whiteBoardView)
    {
        whiteBoardView->onLineColorChanged(m_data->m_color);
        whiteBoardView->onLineWidthChanged(m_data->m_lineWidth);
        whiteBoardView->onDrawTypeChanged(CWB::DRAW_TYPE_UNDEFINE);
        connect(m_data,SIGNAL(sigClear()),whiteBoardView,SLOT(onClear()));
        connect(m_data,SIGNAL(sigDrawTypeChanged(CWB::DrawType)),whiteBoardView,SLOT(onDrawTypeChanged(CWB::DrawType)));
        connect(m_data,SIGNAL(sigLineColorChanged(QColor)),whiteBoardView,SLOT(onLineColorChanged(QColor)));
        connect(m_data,SIGNAL(sigLineWidthChanged(int)),whiteBoardView,SLOT(onLineWidthChanged(int)));
        connect(m_data,SIGNAL(sigUndo()),whiteBoardView,SLOT(onUndo()));
        connect(m_data,SIGNAL(sigRedo()),whiteBoardView,SLOT(onRedo()));
    }
    setMinimumHeight(50);
}

CWhiteBoardEditorBar::~CWhiteBoardEditorBar()
{
    m_data->deleteLater();
    m_data = NULL;
}


void CWhiteBoardEditorBar::setBackgroundColor(const QColor &color)
{
    this->setStyleSheet(QString("QWidget{background-color:%1 ;border: 0px solid #00ff00;}").arg(color.name()));
}

int CWhiteBoardEditorBar::lineWidth() const
{
    return m_data->m_lineWidth;
}

QColor CWhiteBoardEditorBar::lineColor() const
{
    return m_data->m_color;
}

void CWhiteBoardEditorBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

