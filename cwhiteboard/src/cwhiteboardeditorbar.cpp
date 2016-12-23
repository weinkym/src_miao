#include "cwhiteboardeditorbar.h"
#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QInputDialog>
#include "cwhiteboardview.h"
#include <QDebug>

CWhiteBoardEditorBar::CWhiteBoardEditorBar(CWhiteBoardView *whiteBoardView, QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent,f)
    ,m_hLayout(new QHBoxLayout)
    ,m_color(QColor(0,0,0))
    ,m_lineWidth(3)
{
    setBackgroundColor(QColor(62,65,70));
    this->setLayout(m_hLayout);
    initButtons();
    if(whiteBoardView)
    {
        whiteBoardView->onLineColorChanged(m_color);
        whiteBoardView->onLineWidthChanged(m_lineWidth);
        whiteBoardView->onDrawTypeChanged(CWB::DRAW_TYPE_RECT);
        connect(this,SIGNAL(sigClear()),whiteBoardView,SLOT(onClear()));
        connect(this,SIGNAL(sigDrawTypeChanged(CWB::DrawType)),whiteBoardView,SLOT(onDrawTypeChanged(CWB::DrawType)));
        connect(this,SIGNAL(sigLineColorChanged(QColor)),whiteBoardView,SLOT(onLineColorChanged(QColor)));
        connect(this,SIGNAL(sigLineWidthChanged(int)),whiteBoardView,SLOT(onLineWidthChanged(int)));
    }
    setMinimumHeight(50);
}


void CWhiteBoardEditorBar::setBackgroundColor(const QColor &color)
{
    this->setStyleSheet(QString("QWidget{background-color:%1 ;border: 0px solid #00ff00;}").arg(color.name()));
}

void CWhiteBoardEditorBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CWhiteBoardEditorBar::initButtons()
{
    addButton(":/images/eraser_normal_128px.png",":/images/eraser_normal_128px.png",":/images/eraser_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_ERASER);
    addButton(":/images/ellipse_normal_128px.png",":/images/ellipse_pressed_128px.png",":/images/ellipse_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_ELLIPSE);
    addButton(":/images/rect_normal_128px.png",":/images/rect_pressed_128px.png",":/images/rect_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_RECT);
    addButton(":/images/pen_normal_128px.png",":/images/pen_normal_128px.png",":/images/pen_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_PEN);
    addButton(":/images/color_normal_128px.png",":/images/color_normal_128px.png",":/images/color_normal_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_COLOR);
    addButton(":/images/linewidth_normal_128px.png",":/images/linewidth_normal_128px.png",":/images/linewidth_normal_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_LINE_WIDTH);
    QSpacerItem *item = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_hLayout->addSpacerItem(item);
}

void CWhiteBoardEditorBar::addButton(const QString &normalIamgeUrl, const QString &hoverIamgeUrl, const QString &pressedIamgeUrl, CWhiteBoardEditorButton::EditButtonType type)
{
    CWhiteBoardEditorButton *button = new CWhiteBoardEditorButton(normalIamgeUrl,hoverIamgeUrl,pressedIamgeUrl,type);
    QSize size(30,30);
    button->setMinimumSize(size);
    button->setMaximumSize(size);
    connect(button,SIGNAL(sigClicked()),this,SLOT(onButtonClicked()));
    m_hLayout->insertWidget(0,button);
    m_buttons.append(button);
}

void CWhiteBoardEditorBar::onButtonClicked()
{
    CWhiteBoardEditorButton *button = static_cast<CWhiteBoardEditorButton *>(sender());
    if(button == NULL)
    {
        return;
    }
    CWhiteBoardEditorButton::EditButtonType type = button->getType();
    if(type == CWhiteBoardEditorButton::EDIT_TYPE_COLOR)
    {
        QColor color = QColorDialog::getColor(m_color);
        if(color.isValid() && color != m_color)
        {
            emit sigLineColorChanged(color);
        }
        return;
    }
    if(type == CWhiteBoardEditorButton::EDIT_TYPE_LINE_WIDTH)
    {
        bool ok;
        int width = QInputDialog::getInt(0, tr("QInputDialog::getInteger()"),
                                         tr("Percentage:"), m_lineWidth, 0, 10, 1, &ok);
        if(!ok)
        {
            return;
        }
        if(m_lineWidth != width)
        {
            m_lineWidth = width;
            emit sigLineWidthChanged(m_lineWidth);
        }
        return;
    }
    if(type == CWhiteBoardEditorButton::EDIT_TYPE_ERASER)
    {
        emit sigClear();
        return;
    }

    for(auto i: m_buttons)
    {
        i->setSelected(i == button);
    }
    switch (type)
    {
    case CWhiteBoardEditorButton::EDIT_TYPE_ERASER:
        emit sigClear();
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_PEN:
        emit sigDrawTypeChanged(CWB::DRAW_TYPE_PEN);
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_RECT:
        emit sigDrawTypeChanged(CWB::DRAW_TYPE_RECT);
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_ELLIPSE:
        emit sigDrawTypeChanged(CWB::DRAW_TYPE_ELLIPSE);
        break;
    default:
        break;
    }
}
