#include "cdialog.h"
#include <QKeyEvent>
#include <QUuid>

CDialog::CDialog(CDialog::ShowType showType, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
    , m_showType(showType)
    , m_uuid(QUuid::createUuid())
{
    //加了tool 貌似lineedit无法正常输入，需要tab切换窗口后正常 mac
//    this->setAttribute(Qt::WA_TranslucentBackground);
#ifdef Q_OS_MAC
    this->setWindowFlags(Qt::FramelessWindowHint);
#else
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::Sheet);
#endif
    //    setWindowTitle(CLJConfigUtils::getLJLiveString());
}

CDialog::~CDialog()
{
}

QUuid CDialog::getUuid() const
{
    return m_uuid;
}

CDialog::ShowType CDialog::getShowType() const
{
    return m_showType;
}

void CDialog::keyPressEvent(QKeyEvent *event)
{
    if(!event->modifiers() || (event->modifiers() & Qt::KeypadModifier && event->key() == Qt::Key_Enter))
    {
        switch(event->key())
        {
        case Qt::Key_Escape:
            event->ignore();
            return;
            break;
        default:
            break;
        }
    }
    return QDialog::keyPressEvent(event);
}

void CDialog::accept()
{
    QDialog::accept();
    doCallback(m_defaultCallbackObj);
}

void CDialog::closeEvent(QCloseEvent *event)
{
    QDialog::closeEvent(event);
    emit sigClosed(m_uuid, m_showType);
}

void CDialog::setDefaultCallback(CDialog::CallbackObject obj)
{
    m_defaultCallbackObj = obj;
}

void CDialog::doCallback(CDialog::CallbackObject obj)
{
    //todo delayCallbackMS 使用逻辑
    if(obj.callback)
    {
        obj.callback(obj.param, m_uuid);
    }
}
