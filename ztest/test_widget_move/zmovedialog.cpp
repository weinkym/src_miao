#include "zmovedialog.h"
#include <QDebug>
#include <QCoreApplication>

ZMoveDialog::ZMoveDialog(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
    ,m_moveWidget(NULL)
{
#ifdef Q_OS_MAC
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::ToolTip);
#else
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::Sheet);
#endif
    startTimer(50);
}

ZMoveDialog::~ZMoveDialog()
{
    //
}

void ZMoveDialog::setMoveWidget(QWidget *moveWidget)
{
    m_moveWidget = moveWidget;
}

void ZMoveDialog::timerEvent(QTimerEvent *event)
{
    if(isVisible())
    {
//        QWidget *pw = parentWidget();
        if(m_moveWidget)
        {
            QPoint pos = m_moveWidget->mapToGlobal(QPoint(m_moveWidget->pos().x(),m_moveWidget->pos().y()));
//            qDebug()<<this->updatesEnabled();
            if(this->pos() != pos)
            {
//                this->move(pos);
//                this->update();
//                this->adjustPosition();
//            QCoreApplication::processEvents(QEventLoop::AllEvents);
            }
        }
    }
    return QDialog::timerEvent(event);
}
