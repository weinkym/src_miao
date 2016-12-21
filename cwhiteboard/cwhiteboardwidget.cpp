#include "cwhiteboardwidget.h"
#include "ui_cwhiteboardwidget.h"
#include "cwhiteboardview.h"
#include "cwhiteboardeditorbutton.h"

CWhiteBoardWidget::CWhiteBoardWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWhiteBoardWidget)
{
    ui->setupUi(this);
    m_whiteBoardView = new CWhiteBoardView;
    ui->verticalLayout->insertWidget(0,m_whiteBoardView);
    ui->widgetButtons->setStyleSheet(QString("#widgetButtons{min-height:%1px;max-height:%1px;background-color:rgb(62, 65, 70);}").arg(50));
    initButtons();
}

CWhiteBoardWidget::~CWhiteBoardWidget()
{
    delete ui;
}

void CWhiteBoardWidget::initButtons()
{
    addButton(":/images/ellipse_normal_128px.png",":/images/ellipse_pressed_128px.png",":/images/ellipse_pressed_128px.png",1);
    addButton(":/images/rect_normal_128px.png",":/images/rect_pressed_128px.png",":/images/rect_pressed_128px.png",1);
}

void CWhiteBoardWidget::addButton(const QString &normalIamgeUrl, const QString &hoverIamgeUrl, const QString &pressedIamgeUrl, int type)
{
    CWhiteBoardEditorButton *button = new CWhiteBoardEditorButton(normalIamgeUrl,hoverIamgeUrl,pressedIamgeUrl,type);
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
    for(auto i: m_buttons)
    {
        i->setSelected(i == button);
    }
}
