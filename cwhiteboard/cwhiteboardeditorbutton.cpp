#include "cwhiteboardeditorbutton.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

CWhiteBoardEditorButton::CWhiteBoardEditorButton(const QString &normalIamgeUrl, const QString &hoverIamgeUrl, const QString &pressedIamgeUrl, CWhiteBoardEditorButton::EditButtonType type, QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent,f)
    ,m_isPressed(false)
    ,m_selected(false)
    ,m_type(type)
    ,m_normalIamgeUrl(normalIamgeUrl)
    ,m_hoverIamgeUrl(hoverIamgeUrl)
    ,m_pressedIamgeUrl(pressedIamgeUrl)
{
    QStringList styleSheetStringList;
    styleSheetStringList.append(QString("QWidget{border-image: url(%2) 0 0 0 0;}").arg(m_normalIamgeUrl));
    styleSheetStringList.append(QString("QWidget:hover{border-image: url(%1) 0 0 0 0;}").arg(m_hoverIamgeUrl));
    this->setStyleSheet(styleSheetStringList.join("\n"));
}

CWhiteBoardEditorButton::~CWhiteBoardEditorButton()
{

}

void CWhiteBoardEditorButton::setSelected(bool selected)
{
    if(m_selected == selected)
    {
        return;
    }
    m_selected = selected;
    this->setStyleSheet(QString("QWidget{border-image: url(%1) 0 0 0 0;background-color:transparent ;border: 1px solid #1880ed;}").arg(m_selected ? m_pressedIamgeUrl : m_normalIamgeUrl));
}

void CWhiteBoardEditorButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
    }
    else
    {
        m_isPressed = false;
    }
    return QWidget::mousePressEvent(event);
}

void CWhiteBoardEditorButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        m_isPressed = false;
        emit sigClicked();
    }
    return QWidget::mouseReleaseEvent(event);
}

//继承QWidget 且声明Q_OBJECT时，需要重载paintEvent
//具体参考Qt Style Sheets Reference中的QWidget
void CWhiteBoardEditorButton::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
