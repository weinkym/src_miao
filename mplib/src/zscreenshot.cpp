#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QImageWriter>
#include <QPaintEvent>
#include <QDebug>
#include <QDir>
#include <QToolBar>
#include <QPainter>
#include <QToolButton>
#include <QPixmap>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QDesktopServices>
#include <QImageWriter>
#include <QMessageBox>
#include <QClipboard>

#include "zscreenshot.h"

ZScreenshotDlg::ZScreenshotDlg(QWidget *parent):QDialog(parent,Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint)
{
    resize(qApp->desktop()->size());
    m_bgImg = QImage(size(),QImage::Format_ARGB32_Premultiplied);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setCursor(Qt::CrossCursor);
    drawWindow();
    initSelectParam();
    //===================================
    WId id = qApp->desktop()->winId();
    QRect rect = QRect(m_x,m_y,m_width,m_height).normalized();
    qDebug()<<rect.width();
    QPixmap pix = QPixmap();
    pix = QPixmap::grabWindow(id,rect.x(),rect.y(),rect.width(),rect.height());
    m_editor = new ZScreenEditor(pix,this);
    m_editor->hide();
    connect(m_editor,SIGNAL(sigReset()),this,SLOT(onSigReset()));
    connect(m_editor,SIGNAL(sigSelectImg(QPixmap)),this,SLOT(onSigSelectedImg(QPixmap)));
    connect(m_editor,SIGNAL(sigCancel()),this,SLOT(onSigCancel()));
}

QPixmap ZScreenshotDlg::getSelectedImg()
{
    return m_selectImg;
}

void ZScreenshotDlg::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawImage(rect(),m_bgImg);
}

void ZScreenshotDlg::drawWindow()
{
    QPainter painter(&m_bgImg);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(m_bgImg.rect(),QColor(0,0,0,70));
    int penWidth = 2;
    QPen pen = painter.pen();
    pen.setWidth(penWidth);
    painter.setPen(pen);
    painter.fillRect(m_x,m_y,m_width,m_height,Qt::transparent);
    painter.drawRect(m_x - penWidth,m_y - penWidth,m_width + 2 * penWidth,m_height + 2 * penWidth);
    repaint();//即时强制重绘
}

void ZScreenshotDlg::mouseMoveEvent(QMouseEvent *e)
{
    if(!m_bGrabing){
        QWidget::mouseMoveEvent(e);
        return;
    }
    if(e->buttons() & Qt::LeftButton)
    {
        QPoint pos = e->pos();
        m_width = pos.x() - m_x;
        m_height = pos.y() - m_y;
        drawWindow();
    }
}

void ZScreenshotDlg::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        qDebug()<<e->pos();
        if(!m_bGrabing){
            QWidget::mousePressEvent(e);
            return;
        }
        m_x = e->pos().x();
        m_y = e->pos().y();
    }
    else if(e->button() == Qt::RightButton)
    {
        if(m_editor->isHidden())
        {
            this->reject();
        }
        else
        {
            m_editor->hide();
            onSigReset();
        }
    }
}


void ZScreenshotDlg::mouseReleaseEvent(QMouseEvent *e)
{
    if(!m_bGrabing)
    {
        QWidget::mouseReleaseEvent(e);
        return;
    }
    if(e->button() == Qt::LeftButton)
    {
        m_bGrabing = false;
        setCursor(Qt::ArrowCursor);
        WId id = qApp->desktop()->winId();
        QRect rect = QRect(m_x,m_y,m_width,m_height).normalized();
        qDebug()<<rect.width();
        QPixmap pix = QPixmap();
        pix = QPixmap::grabWindow(id,rect.x(),rect.y(),rect.width(),rect.height());
//        ImageEditor* editor = new ImageEditor(QPixmap(),this);
        m_editor->resetByImg(pix);
        m_editor->move(rect.topLeft());
        m_editor->show();
    }
}

void ZScreenshotDlg::initSelectParam()
{
    m_bGrabing = true;
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
}

void ZScreenshotDlg::onSigReset()
{
    initSelectParam();
    drawWindow();
}

void ZScreenshotDlg::onSigSelectedImg(const QPixmap &pix)
{
    QClipboard* clb = QApplication::clipboard();
    clb->setPixmap(pix);
//    clb->setImage(pix.toImage());
    m_selectImg = pix;
    this->accept();
}

void ZScreenshotDlg::onSigCancel()
{
//    initSelectParam();
//    drawWindow();
    this->reject();
}

//====================================================================
ZScreenEditor::ZScreenEditor(const QPixmap& img, QWidget *parent)
    :QWidget(parent),m_image(img)
{
    updateForImg(img);
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setHidden(true);
    m_lineEdit->setStyleSheet("QLineEdit{background:rgba(255, 255, 0, 0)}");

    initToolBar();
    updateToolBar();
}

void ZScreenEditor::initToolBar(){
    toolBar = new QToolBar(this);
    QPixmap pix(BTN_DEFAULT_SIZE,BTN_DEFAULT_SIZE);
    QPainter painter(&pix);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    QIcon icon;

    QButtonGroup* group = new QButtonGroup(this);
    icon = QIcon(":/images/pen.png");
    addButton(icon,tr("画笔"),group,DrawFree);

    icon = QIcon(":/images/text.png");
    addButton(icon,tr("文本"),group,DrawText);

    painter.fillRect(pix.rect(),QColor(255,255,255,150));
    QPen pen = painter.pen();
    pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawEllipse(5,5,pix.width() - 10,pix.height() - 10);
    icon = QIcon(pix);
    addButton(icon,tr("椭圆工具"),group,DrawCircle);

    painter.fillRect(pix.rect(),QColor(255,255,255,150));
    painter.drawRect(8,8,pix.width() - 16,pix.height() - 16);
    icon = QIcon(pix);
    addButton(icon,tr("矩形工具"),group,DrawRect);

    icon = QIcon(":/images/undo.png");
    addButton(icon,tr("撤销"),group,UNDO);

    icon = QIcon(":/images/saveas.png");
    //另存为
    addButton(icon,tr("Save as"),group,SAVEAS);

    icon = QIcon(":/images/cancel.png");
    addButton(icon,tr("取消"),group,CANCEL);

    icon = QIcon(":/images/complete.png");
    addButton(icon,tr("确定"),group,OK);

    connect(group,SIGNAL(buttonClicked(int)),this,SLOT(onBtnClicked(int)));
}

//void ZScreenEditor::moveEvent(QMoveEvent* e)
//{
//    if(frameGeometry().bottom() + toolBar->height() > qApp->desktop()->height()){
//        resize(m_image.size());
//        toolBar->move(0,m_image.height() - toolBar->height());
//    }
//}

void ZScreenEditor::mouseDoubleClickEvent(QMouseEvent *)
{
    onCompleteBtnClicked();
}

void ZScreenEditor::resetByImg(const QPixmap &img)
{
    updateForImg(img);
    updateToolBar();
    repaint();
}

QPixmap ZScreenEditor::getSelectedImg()
{
    QPixmap pix = QPixmap();
    pix = QPixmap::grabWidget(this,m_image.rect());
    return pix;
}

void ZScreenEditor::updateToolBar()
{
    toolBar->move(0,m_image.height());
//    setMinimumSize(250,m_image.height() + toolBar->height());
    setMinimumSize(toolBar->children().count() * BTN_DEFAULT_SIZE,m_image.height() + toolBar->height());
}

void ZScreenEditor::updateForImg(const QPixmap &img)
{
    m_image = img;

    resize(img.width(),img.height() + 40);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setMouseTracking(true);
    m_tempImg = QImage(img.size(),QImage::Format_ARGB32_Premultiplied);
    m_tempImg.fill(Qt::transparent);
    m_bufferImg = m_tempImg;
    m_stackImages.clear();
    m_stackImages.push_back(m_tempImg);
     m_x = 0;
     m_y = 0;
     m_w = 0;
     m_h = 0;
}

void ZScreenEditor::paint()
{
    QPainter painter(&m_tempImg);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPen pen = painter.pen();
    pen.setWidth(2);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    switch(m_curOption){
    case NoOption:
        break;
    case DrawFree:
        painter.drawLine(m_oldPos,m_curPos);
        break;
    case DrawRect:
        painter.drawRect(m_x,m_y,m_w,m_h);
        break;
    case DrawCircle:
       painter.drawEllipse(m_x,m_y,m_w,m_h);
       break;
    }
    update();
}

void ZScreenEditor::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::LeftButton){
        if(!m_image.rect().contains(e->pos())){
            QWidget::mousePressEvent(e);
            return;
        }
        m_bDrawing = true;
        m_x = e->pos().x();
        m_y = e->pos().y();
        m_curPos = e->pos();
        switch(m_curOption){
        case NoOption:break;
        case DrawRect:
        case DrawCircle:
            setCursor(Qt::CrossCursor);
            break;
        case DrawFree:
        {
            QPixmap pix(":/images/pen.png");
            QCursor cursor(pix,0,pix.height());
            setCursor(cursor);
        }
            break;
        case DrawText:
        {
            QRect rect(e->x(),e->y(),200,50);
            m_lineEdit->setGeometry(rect);
            m_lineEdit->setHidden(false);

        }
            break;
        }
    }else if(e->button() == Qt::RightButton){
        emit sigReset();
        this->close();
    }

}

void ZScreenEditor::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        if(!m_image.rect().contains(e->pos())){
            QWidget::mouseMoveEvent(e);
            return;
        }
        switch(m_curOption){
        case DrawFree:
            m_oldPos = m_curPos;
            m_curPos = e->pos();
            break;
        case DrawRect:
        case DrawCircle:
            m_tempImg = m_bufferImg;
            m_w = e->pos().x() - m_x;
            m_h = e->pos().y() - m_y;
            break;
        case NoOption:
        default:
            break;
        }
        paint();
    }
}

void ZScreenEditor::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        m_bDrawing = false;
        m_bufferImg = m_tempImg;
        m_w = 0;
        m_h = 0;
        m_stackImages.push_back(m_tempImg);
    }
}
void ZScreenEditor::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawPixmap(0,0,m_image);
    if(m_bDrawing){
        painter.drawImage(0,0,m_tempImg);
    }else{
        painter.drawImage(0,0,m_bufferImg);
    }
}

void ZScreenEditor::undo()
{
    if(m_stackImages.size() <= 1) return;

    m_stackImages.pop_back();
    m_tempImg = m_stackImages.last();
    m_bufferImg = m_stackImages.last();
    update();
}

bool ZScreenEditor::save(const QString& path)
{
    QPixmap pix = QPixmap();
    pix = QPixmap::grabWidget(this,m_image.rect());
    return pix.save(path);
}

void ZScreenEditor::addButton(const QIcon &icon, const QString &text, QButtonGroup *group, int id)
{
    QToolButton* btn = new QToolButton;
//    icon = QIcon(":/images/pen.png");
    btn->setIcon(icon);
    btn->setCursor(Qt::ArrowCursor);
    toolBar->addWidget(btn);
    btn->setCheckable(true);
    btn->setToolTip(text);
    group->addButton(btn,id);
//    connect(btn,SIGNAL(clicked()),this,SLOT(curOptionChanged()));
}

void ZScreenEditor::saveAs()
{
    QString filter = "*.png;;*.jpg;;*.bmp;;*.gif;;*.jpeg";
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString dir = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation) +QDir::separator() +  "grabbedImage.png";
#else
    QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +QDir::separator() +  "grabbedImage.png";
#endif
    QString path = QFileDialog::getSaveFileName(0,tr("Save"),dir,filter);
    if(!path.isNull()){
        if(!save(path)){
            QMessageBox::warning(this,tr("failed to save"),tr("Sorry! Save failed!Please check savePath!"));
        }else{
//            this->rej
        }
    }
}

void ZScreenEditor::onCancelBtnClicked()
{
    this->close();
    emit sigCancel();
}

void ZScreenEditor::onCompleteBtnClicked()
{
    QPixmap pix = getSelectedImg();
    emit sigSelectImg(pix);
    this->close();
}

void ZScreenEditor::onBtnClicked(int id)
{
//    m_curOption = id;
    switch (id) {
    case SAVEAS:
        saveAs();
        break;
    case UNDO:
        undo();
        break;
    case OK:
        onCompleteBtnClicked();
        break;
    case CANCEL:
        onCancelBtnClicked();
        break;

    case DrawFree:
    {
        m_curOption = DrawFree;
        QPixmap pix(":/icons/pen.png");
        QCursor cursor(pix,0,pix.height());
        setCursor(cursor);
    }
        break;
    case DrawCircle:
    {
        m_curOption = DrawCircle;
        setCursor(Qt::CrossCursor);
    }
        break;
    case DrawRect:
    {
        m_curOption = DrawRect;
        setCursor(Qt::CrossCursor);
    }
        break;
    case DrawText:
    {
        m_curOption = DrawText;
        setCursor(Qt::CrossCursor);
    }
        break;
    default:
        break;
    }
}
