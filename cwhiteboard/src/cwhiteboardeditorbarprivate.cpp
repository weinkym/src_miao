#include "cwhiteboardeditorbarprivate.h"
#include "cwhiteboardeditorbar.h"
#include <QHBoxLayout>
#include <QColorDialog>
#include <QInputDialog>

CWhiteBoardEditorBarPrivate::CWhiteBoardEditorBarPrivate()
    :m_hLayout(new QHBoxLayout)
    ,m_color(QColor(255,0,0))
    ,m_lineWidth(1)
{
    initButtons();
}

void CWhiteBoardEditorBarPrivate::initButtons()
{
//    addButton(":/images/eraser_normal_128px.png",":/images/eraser_normal_128px.png",":/images/eraser_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_ERASER);
    addButton(":/images/eraser_normal_128px.png",":/images/eraser_pressed_128px.png",":/images/eraser_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_CLEAR);
    addButton(":/images/redo_normal_128px.png",":/images/redo_pressed_128px.png",":/images/redo_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_REDO);
    addButton(":/images/undo_normal_128px.png",":/images/undo_pressed_128px.png",":/images/undo_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_UNDO);
    addButton(":/images/arrow_normal_128px.png",":/images/arrow_pressed_128px.png",":/images/arrow_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_ARROW);
    /*CWhiteBoardEditorButton *rectButton = */addButton(":/images/rect_normal_128px.png",":/images/rect_pressed_128px.png",":/images/rect_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_RECT);
    addButton(":/images/ellipse_normal_128px.png",":/images/ellipse_pressed_128px.png",":/images/ellipse_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_ELLIPSE);
    addButton(":/images/text_normal_128px.png",":/images/text_pressed_128px.png",":/images/text_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_TEXT);
    addButton(":/images/line_normal_128px.png",":/images/line_pressed_128px.png",":/images/line_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_LINE);
    addButton(":/images/pen_normal_128px.png",":/images/pen_pressed_128px.png",":/images/pen_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_PEN);
    addButton(":/images/point_normal_128px.png",":/images/point_pressed_128px.png",":/images/point_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_POINT);

    //===================
    addButton(":/images/color_normal_128px.png",":/images/color_normal_128px.png",":/images/color_normal_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_COLOR);
//    addButton(":/images/linewidth_normal_128px.png",":/images/linewidth_pressed_128px.png",":/images/linewidth_pressed_128px.png",CWhiteBoardEditorButton::EDIT_TYPE_LINE_WIDTH);

    QSpacerItem *item = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_hLayout->addSpacerItem(item);
//    emit rectButton->sigClicked();
}

CWhiteBoardEditorButton *CWhiteBoardEditorBarPrivate::addButton(const QString &normalIamgeUrl, const QString &hoverIamgeUrl, const QString &pressedIamgeUrl, CWhiteBoardEditorButton::EditButtonType type)
{
    CWhiteBoardEditorButton *button = new CWhiteBoardEditorButton(normalIamgeUrl,hoverIamgeUrl,pressedIamgeUrl,type);
    QSize size(30,30);
    button->setMinimumSize(size);
    button->setMaximumSize(size);
    connect(button,SIGNAL(sigClicked()),this,SLOT(onButtonClicked()));
    m_hLayout->insertWidget(0,button);
    m_buttons.append(button);
    return button;
}

void CWhiteBoardEditorBarPrivate::onButtonClicked()
{
    CWhiteBoardEditorButton *button = static_cast<CWhiteBoardEditorButton *>(sender());
    if(button == NULL)
    {
        return;
    }
    CWhiteBoardEditorButton::EditButtonType type = button->getType();
    if(type == CWhiteBoardEditorButton::EDIT_TYPE_COLOR)
    {
//        QColor color = QColorDialog::getColor(m_color,0,"",QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
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
        int width = 1;
        //采用标准的无法汉化按钮 QInputDialog::getInt
        QInputDialog dialog(0,Qt::WindowFlags());
        //设置绘制线宽
        dialog.setWindowTitle(tr("Set drawwidth"));
        //线宽:
        dialog.setLabelText(tr("Line width:"));
        dialog.setIntRange(0, 10);
        dialog.setIntValue(1);
        dialog.setIntStep(1);
        dialog.setCancelButtonText(tr("Cancle"));
        dialog.setOkButtonText(tr("Ok"));
        int ret = dialog.exec();
        ok = !!ret;
        if (ret) {
            width = dialog.intValue();
        }

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
    if(type == CWhiteBoardEditorButton::EDIT_TYPE_CLEAR)
    {
        emit sigClear();
        return;
    }
    if(type == CWhiteBoardEditorButton::EDIT_TYPE_UNDO)
    {
        emit sigUndo();
        return;
    }

    if(type == CWhiteBoardEditorButton::EDIT_TYPE_REDO)
    {
        emit sigRedo();
        return;
    }

    for(auto i: m_buttons)
    {
        i->setSelected(i == button);
    }
    switch (type)
    {
    case CWhiteBoardEditorButton::EDIT_TYPE_ARROW:
        emit sigDrawTypeChanged(CWB::DRAW_TYPE_ARROW);
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_ERASER:
        emit sigDrawTypeChanged(CWB::DRAW_TYPE_ERASER);
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
    case CWhiteBoardEditorButton::EDIT_TYPE_TEXT:
        emit sigDrawTypeChanged(CWB::DRAW_TYPE_TEXT);
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_LINE:
        emit sigDrawTypeChanged(CWB::DRAW_TYPE_LINE);
        break;
    case CWhiteBoardEditorButton::EDIT_TYPE_POINT:
        emit sigDrawTypeChanged(CWB::DRAW_TYPE_POINT);
        break;
    default:
        break;
    }
}
