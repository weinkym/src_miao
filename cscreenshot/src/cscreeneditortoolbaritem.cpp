#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "cscreeneditortoolbaritem.h"
#include "cscreeneditorbuttonitem.h"
#include "cscreenattributetoolbaritem.h"

#include <QDebug>

CScreenEditorToolbarItem::CScreenEditorToolbarItem(QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_rectButtonItem(NULL)
    ,m_cancelButtonItem(NULL)
    ,m_okButtonItem(NULL)
    ,m_attributeToolbarItem(NULL)
    ,m_rect(0,0,300,m_toolbarHeight)
    ,m_currentButtonType(CSCREEN_BUTTON_TYPE_UNDEFINED)
{
    C_SCREENSHOTSHARED_LOG_TIMER_FUNCTION;
    qreal x = m_buttonMargin;
    qreal y = 0.5 * (m_toolbarHeight - m_buttonHeight);

    {
        CScreenEditorButtonItem::Param param;
        param.normalImage=QImage(":/screenshot/res/screenshot/rect_icon_normal.png");
        param.pressedImage=QImage(":/screenshot/res/screenshot/rect_icon_pressed.png");
        param.rect=QRectF(x,y,m_buttonWidth,m_buttonHeight);
        param.type=CSCREEN_BUTTON_TYPE_RECT;
        CScreenEditorButtonItem *item = new CScreenEditorButtonItem(param,this);
        connect(item,SIGNAL(sigClicked(CScreenButtonType)),this,SLOT(onButtonClicked(CScreenButtonType)));
        m_buttonItemList.append(item);
    }
    x += m_buttonMargin + m_buttonWidth;
    //============
    {
        CScreenEditorButtonItem::Param param;
        param.normalImage=QImage(":/screenshot/res/screenshot/ellipse_icon_normal.png");
        param.pressedImage=QImage(":/screenshot/res/screenshot/ellipse_icon_pressed.png");
        param.rect=QRectF(x,y,m_buttonWidth,m_buttonHeight);
        param.type=CSCREEN_BUTTON_TYPE_ELLIPSE;
        CScreenEditorButtonItem *item = new CScreenEditorButtonItem(param,this);
        connect(item,SIGNAL(sigClicked(CScreenButtonType)),this,SLOT(onButtonClicked(CScreenButtonType)));
        m_buttonItemList.append(item);
    }
    x += m_buttonMargin + m_buttonWidth;

    //============
    {
        CScreenEditorButtonItem::Param param;
        param.normalImage=QImage(":/screenshot/res/screenshot/rect_icon_normal.png");
        param.pressedImage=QImage(":/screenshot/res/screenshot/rect_icon_pressed.png");
        param.rect=QRectF(x,y,m_buttonWidth,m_buttonHeight);
        param.type=CSCREEN_BUTTON_TYPE_RECT;
        CScreenEditorButtonItem *item = new CScreenEditorButtonItem(param,this);
        connect(item,SIGNAL(sigClicked(CScreenButtonType)),this,SLOT(onButtonClicked(CScreenButtonType)));
        m_buttonItemList.append(item);
    }
    x += m_buttonMargin + m_buttonWidth;
    //============
    {
        CScreenEditorButtonItem::Param param;
        param.normalImage=QImage(":/screenshot/res/screenshot/cancel_icon_normal.png");
        param.pressedImage=QImage(":/screenshot/res/screenshot/cancel_icon_normal.png");
        param.rect=QRectF(x,y,m_buttonWidth,m_buttonHeight);
        param.type=CSCREEN_BUTTON_TYPE_CANCLE;
        CScreenEditorButtonItem *item = new CScreenEditorButtonItem(param,this);
        connect(item,SIGNAL(sigClicked(CScreenButtonType)),this,SLOT(onButtonClicked(CScreenButtonType)));
        m_buttonItemList.append(item);
    }
    x += m_buttonMargin + m_buttonWidth;
    //============
    {
        CScreenEditorButtonItem::Param param;
        param.normalImage=QImage(":/screenshot/res/screenshot/confirm_icon_normal.png");
        param.pressedImage=QImage(":/screenshot/res/screenshot/confirm_icon_normal.png");
        param.rect=QRectF(x,y,m_buttonWidth,m_buttonHeight);
        param.type=CSCREEN_BUTTON_TYPE_OK;
        CScreenEditorButtonItem *item = new CScreenEditorButtonItem(param,this);
        connect(item,SIGNAL(sigClicked(CScreenButtonType)),this,SLOT(onButtonClicked(CScreenButtonType)));
        m_buttonItemList.append(item);
    }
    x += m_buttonMargin + m_buttonWidth;

    m_rect.setWidth(x);
    {
        m_attributeToolbarItem = new CScreenAttributeToolbarItem(this);
        m_attributeToolbarItem->setZValue(this->zValue() + 1);
        m_attributeToolbarItem->setPos(0,this->boundingRect().height());
        m_attributeToolbarItem->setVisible(false);
    }
}

CScreenEditorToolbarItem::~CScreenEditorToolbarItem()
{

}

QRectF CScreenEditorToolbarItem::boundingRect() const
{
    return m_rect;
}

void CScreenEditorToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawImage(m_rect,QImage(":/screenshot/res/screenshot/toolbarbg.png"));
}

CScreenButtonType CScreenEditorToolbarItem::getCurrentButtonType()
{
    return m_currentButtonType;
}

int CScreenEditorToolbarItem::getLineWidth() const
{
    return m_attributeToolbarItem->getLineWidth();
}

QColor CScreenEditorToolbarItem::getColor() const
{
    return m_attributeToolbarItem->getColor();
}

void CScreenEditorToolbarItem::initScreenEditorButtonItems()
{
    //
}

void CScreenEditorToolbarItem::updateAttributeToolbar(const qreal centerX)
{
    QGraphicsScene *scene = this->scene();
    if(scene == NULL)
    {
        return;
    }
    QList<QGraphicsView *> views = scene->views();
    if(views.isEmpty())
    {
        return;
    }
    qreal veiwHeight = views.first()->height();
    qreal viewWidth = views.first()->width();
    QPointF maxPoint = this->mapFromScene(QPointF(viewWidth,veiwHeight));
    C_SCREENSHOTSHARED_LOG(QString("pos(%1,%2),viewWidth %3,veiwHeight %4,maxPoint(%5,%6)")
             .arg(this->pos().x()).arg(this->pos().y())
             .arg(viewWidth).arg(veiwHeight)
             .arg(maxPoint.x()).arg(maxPoint.y()));

    qreal dx = centerX - m_attributeToolbarItem->boundingRect().width() / 2 ;
    qreal x = dx;
    if(dx < C_SCREEN_ZERO)
    {
        x = 0;
    }
    if(x + m_attributeToolbarItem->boundingRect().width() > maxPoint.x())
    {
        x = maxPoint.x() - m_attributeToolbarItem->boundingRect().width();
    }
    dx =  centerX - (x + 0.5 * m_attributeToolbarItem->boundingRect().width());

    qreal overlapHeight = 0.5 * (m_toolbarHeight - m_buttonWidth);
    qreal y = m_rect.height() - overlapHeight;
    qreal bottomY = y + m_attributeToolbarItem->boundingRect().height() + m_marginAttributeToolbarWidthBottom;

    CScreenArrowDirection arrowDirection = CSCREEN_ARROW_DIRECTION_UP;
    if(bottomY > maxPoint.y())
    {
        y = overlapHeight - m_attributeToolbarItem->boundingRect().height();
        arrowDirection = CSCREEN_ARROW_DIRECTION_DOWN;
    }
    m_attributeToolbarItem->setArrowDirectionPosition(arrowDirection,dx);
    m_attributeToolbarItem->setPos(x,y);
    m_attributeToolbarItem->setVisible(true);
}

void CScreenEditorToolbarItem::onButtonClicked(CScreenButtonType type)
{
    CScreenEditorButtonItem *buttonItem = dynamic_cast<CScreenEditorButtonItem*>(sender());
    if(buttonItem == NULL)
    {
        C_SCREENSHOTSHARED_LOG(QString("buttonItem is NULL"));
        return;
    }

    m_currentButtonType = type;

    if(!(type == CSCREEN_BUTTON_TYPE_CANCLE || type == CSCREEN_BUTTON_TYPE_OK))
    {
        updateAttributeToolbar(buttonItem->boundingRect().center().x());
    }
    foreach (CScreenEditorButtonItem *item, m_buttonItemList)
    {
        item->setSelected(buttonItem == item);
    }
    emit sigButtonClicked(m_currentButtonType);
    C_SCREENSHOTSHARED_LOG(QString("onButtonClicked"));
}
