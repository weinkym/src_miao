#include "cwhiteboardwidget.h"
#include "ui_cwhiteboardwidget.h"
#include "cwhiteboardview.h"
#include "cwhiteboardeditorbutton.h"
#include <QColorDialog>
#include <QInputDialog>

CWhiteBoardEditorButton *createButton(const QString &normalIamgeUrl,const QString &hoverIamgeUrl,const QString &pressedIamgeUrl,CWhiteBoardEditorButton::EditButtonType type)
{
    CWhiteBoardEditorButton *button = new CWhiteBoardEditorButton(normalIamgeUrl,hoverIamgeUrl,pressedIamgeUrl,type);
    QSize size(30,30);
    button->setMinimumSize(size);
    button->setMaximumSize(size);
    return button;
}

CWhiteBoardWidget::CWhiteBoardWidget(QWidget *parent)\
    :QWidget(parent)
    ,ui(new Ui::CWhiteBoardWidget)
    ,m_whiteBoardView(NULL)
    ,m_color(QColor("#000000"))
    ,m_lineWidth(3)
{
    ui->setupUi(this);
    m_whiteBoardView = new CWhiteBoardView;
    m_whiteBoardView->setLineColor(m_color);
    m_whiteBoardView->setLineWidth(m_lineWidth);
    ui->verticalLayout->insertWidget(0,m_whiteBoardView);
    ui->widgetButtons->setStyleSheet(QString("#widgetButtons{min-height:%1px;max-height:%1px;background-color:rgb(62, 65, 70);border: 0px solid #00ff00;}").arg(50));
    initButtons();
}

CWhiteBoardWidget::~CWhiteBoardWidget()
{
    delete ui;
}

void CWhiteBoardWidget::initButtons()
{
    CWhiteBoardEditorButton *button = createButton(":/images/eraser_normal_128px.png",":/images/eraser_normal_128px.png",":/images/eraser_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_ERASER);
    addButton(button);
    button = createButton(":/images/ellipse_normal_128px.png",":/images/ellipse_pressed_128px.png",":/images/ellipse_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_ELLIPSE);
    addButton(button);
    button = createButton(":/images/rect_normal_128px.png",":/images/rect_pressed_128px.png",":/images/rect_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_RECT);
    addButton(button);
    emit button->sigClicked();
    button = createButton(":/images/pen_normal_128px.png",":/images/pen_normal_128px.png",":/images/pen_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_PEN);
    addButton(button);
    button = createButton(":/images/color_normal_128px.png",":/images/color_normal_128px.png",":/images/color_normal_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_COLOR);
    addButton(button);
    button = createButton(":/images/linewidth_normal_128px.png",":/images/linewidth_normal_128px.png",":/images/linewidth_normal_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_LINE_WIDTH);
    addButton(button);
    QSpacerItem *horizontalSpacer = new QSpacerItem(15,2,QSizePolicy::Fixed,QSizePolicy::Fixed);
    ui->hLayout->insertItem(0,horizontalSpacer);
    ui->hLayout->setSpacing(15);
}

void CWhiteBoardWidget::addButton(CWhiteBoardEditorButton *button)
{
    if(button == NULL)
    {
        return;
    }
    QSize size(30,30);
    button->setMinimumSize(size);
    button->setMaximumSize(size);
    ui->hLayout->insertWidget(0,button);
    m_buttons.append(button);
    connect(button,SIGNAL(sigClicked()),this,SLOT(onButtonClicked()));
}

void CWhiteBoardWidget::onButtonClicked()
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
            m_whiteBoardView->setLineColor(color);
        }
        return;
    }
    if(type == CWhiteBoardEditorButton::EDIT_TYPE_LINE_WIDTH)
    {
        bool ok;
        int width = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"),
                                         tr("Percentage:"), m_lineWidth, 0, 10, 1, &ok);
        if(!ok)
        {
            return;
        }
        if(m_lineWidth != width)
        {
            m_lineWidth = width;
            m_whiteBoardView->setLineWidth(m_lineWidth);
        }
        return;
    }
    if(type == CWhiteBoardEditorButton::EDIT_TYPE_ERASER)
    {
        m_whiteBoardView->clear();
        return;
    }

    for(auto i: m_buttons)
    {
        i->setSelected(i == button);
    }
    switch (type)
    {
    case CWhiteBoardEditorButton::EDIT_TYPE_ERASER:
        m_whiteBoardView->clear();
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_PEN:
        m_whiteBoardView->setDrawType(CWB::DRAW_TYPE_PEN);
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_RECT:
        m_whiteBoardView->setDrawType(CWB::DRAW_TYPE_RECT);
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_ELLIPSE:
        m_whiteBoardView->setDrawType(CWB::DRAW_TYPE_ELLIPSE);
        break;
    default:
        break;
    }
}
