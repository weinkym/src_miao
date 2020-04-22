#include "cbasedialog.h"
#include "clogsetting.h"
#include <QKeyEvent>
#include <QUuid>

CBaseDialog::CBaseDialog(CBaseDialog::ShowType showType, QWidget *parent, Qt::WindowFlags f)
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

CBaseDialog::~CBaseDialog()
{
}

QUuid CBaseDialog::getUuid() const
{
    return m_uuid;
}

CBaseDialog::ShowType CBaseDialog::getShowType() const
{
    return m_showType;
}

void CBaseDialog::startShow()
{
    C_LOG_INFO(QString("dialog show,%1").arg(this->getDescription()));
    this->hide();
    switch(m_showType)
    {
    case SHOW_TYPE_NORMAL:
    {
        this->setModal(false);
        this->show();
        break;
    }
    case SHOW_TYPE_HALF_MODAL:
    {
        this->setModal(true);
        this->show();
        break;
    }
    case SHOW_TYPE_NORMAL_MODAL:
    {
        this->setModal(false);
        this->exec();
        break;
    }
    default:
        break;
    }
    this->show();
}

QString CBaseDialog::getDescription() const
{
    QString className = this->metaObject()->className();
    return C_LOG_P3(className, m_showType, m_uuid);
}

void CBaseDialog::keyPressEvent(QKeyEvent *event)
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

void CBaseDialog::accept()
{
    QDialog::accept();
    doCallback(m_defaultCallbackObj);
}

void CBaseDialog::closeEvent(QCloseEvent *event)
{
    C_LOG_INFO(QString("dialog close,%1").arg(this->getDescription()));
    QDialog::closeEvent(event);
    emit sigClosed(m_uuid, m_showType);
}

void CBaseDialog::setDefaultCallback(CBaseDialog::CallbackObject obj)
{
    m_defaultCallbackObj = obj;
}

void CBaseDialog::doCallback(CBaseDialog::CallbackObject obj)
{
    //todo delayCallbackMS 使用逻辑
    if(obj.callback)
    {
        obj.callback(obj.param, m_uuid);
    }
}
