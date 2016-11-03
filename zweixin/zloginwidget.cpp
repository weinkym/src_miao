#include "zloginwidget.h"
#include <QDomDocument>
#include "ui_zloginwidget.h"
#include "zpublic.h"

ZLoginWidget::ZLoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::zloginwidget)
{
    ui->setupUi(this);
    m_tip = 1;
    m_timer.setInterval(1000);
    m_timer.setSingleShot(true);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onRequestWaitLogin()));
}

ZLoginWidget::~ZLoginWidget()
{
    delete ui;
}

void ZLoginWidget::requestUuid()
{
    LOG_FUNCTION;
    ZRequestAction *action = ZPublicAction::createLoginUuidAction();
    connect(action,SIGNAL(sigFinished(ZRequestAction::ZRequestResponse)),this,SLOT(onFinished(ZRequestAction::ZRequestResponse)));
    action->trigger();
}

void ZLoginWidget::requestQrCode(const QString &uuid)
{
    LOG_FUNCTION;
    ZRequestAction *action = ZPublicAction::createQrCodeAction(uuid);
    connect(action,SIGNAL(sigFinished(ZRequestAction::ZRequestResponse)),this,SLOT(onFinished(ZRequestAction::ZRequestResponse)));
    action->trigger();
}

void ZLoginWidget::requestWaitLogin(const QString &uuid, int tip)
{
    LOG_FUNCTION;
    ZRequestAction *action = ZPublicAction::createWaitLoginAction(uuid,tip);
    connect(action,SIGNAL(sigFinished(ZRequestAction::ZRequestResponse)),this,SLOT(onFinished(ZRequestAction::ZRequestResponse)));
    action->trigger();
}

void ZLoginWidget::requestCookie(const QString &uuid, const QString &ticket, const QString &scan)
{
    LOG_FUNCTION;
    ZRequestAction *action = ZPublicAction::createCookieAction(uuid,ticket,scan);
    connect(action,SIGNAL(sigFinished(ZRequestAction::ZRequestResponse)),this,SLOT(onFinished(ZRequestAction::ZRequestResponse)));
    action->trigger();
}

QString ZLoginWidget::parseUuid(const QByteArray &byteArray)
{
    LOG_FUNCTION;
    QString uuid;
    QString tempData = QString(byteArray).simplified();
    LOG_TEST(QString("TTTTTTTV:tempData=%1").arg(tempData));
    QString key = "code=200";
//    if(tempData.contains(key))
    {
        int index1 = tempData.indexOf("\"");
        int index2 = tempData.lastIndexOf("\"");
        if(index1 != index2 && index1 >= 0 && index2 >= 0)
        {
            uuid = tempData.mid(index1+1,index2-index1-1);
        }
        LOG_TEST(QString("TTTTTTTV:index1=%1").arg(index1));
        LOG_TEST(QString("TTTTTTTV:index2=%1").arg(index2));

//        QString tempStr =
    }
    LOG_TEST(QString("TTTTTTTV:uuid=%1").arg(uuid));
    return uuid;
}

bool ZLoginWidget::parseRedirectUri(const QByteArray &byteArray)
{
    //(window.redirect_uri)=\"([\w\-\.,@?^=%&amp;:/~\+#]*[\w\-\@?^=%&amp;/~\+#])?
    QList<QStringList> redirecttUrlData = Zpublic::regexCapture(QString(byteArray),"(window.redirect_uri)=\\\"([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?");
    LOG_TEST(QString("redirecttUrlData.count()=%1").arg(redirecttUrlData.count()));
    if(redirecttUrlData.count() != 1)
    {
        return false;
    }
    QStringList redirectUrlCapture = redirecttUrlData.first();
    LOG_TEST(QString("redirectUrlCapture.count()=%1").arg(redirectUrlCapture.count()));
    if(redirectUrlCapture.count() <= 2)
    {
        return false;
    }
    QString redirectUrl = redirectUrlCapture.at(2);
    LOG_TEST(QString("redirectUrl=%1").arg(redirectUrl));

    m_scan = parseUrlParam(redirectUrl,"scan");
    m_ticket = parseUrlParam(redirectUrl,"ticket");
    return !(m_scan.isEmpty() || m_ticket.isEmpty());
}

QString ZLoginWidget::parseUrlParam(const QString &url, const QString &param)
{
    QList<QStringList> paramData = Zpublic::regexCapture(url,QString("(%1)=([\\w\\-\\.,@?^=%;:/~\\+#]+)?").arg(param));
    QString result;
    if(paramData.isEmpty())
    {
        return result;
    }
    QStringList paramCapture = paramData.first();
    if(paramCapture.count() <= 2)
    {
        return result;
    }
    result = paramCapture.at(2);
    return result;
}

bool ZLoginWidget::parseCookieData(const QByteArray &byteArray)
{
    QDomDocument doc;
    QString errorString;
    bool ok = doc.setContent(byteArray,&errorString);
    if(!ok)
    {
        LOG_ERROR(errorString);
        return false;
    }
    QDomElement root = doc.documentElement();
    QDomNodeList domNodeList = root.childNodes();
    int count = domNodeList.count();

    for(int i = 0; i < count; ++i)
    {
        QDomNode domNode = domNodeList.at(i);
        QDomElement element = domNode.toElement();
        LOG_TEST(QString("nodeName=%1,%2,%3").arg(element.tagName()).arg(element.nodeName()).arg(element.text()));
        if(element.tagName() == "isgrayscale")
        {
            m_cookieParam.isgrayscale = element.text().toInt();
        }
        else if(element.tagName() == "ret")
        {
            m_cookieParam.ret = element.text().toInt();
        }
        else if(element.tagName() == "skey")
        {
            m_cookieParam.skey = element.text();
        }
        else if(element.tagName() == "wxsid")
        {
            m_cookieParam.wxsid = element.text();
        }
        else if(element.tagName() == "wxuin")
        {
            m_cookieParam.wxuin = element.text();
        }
        else if(element.tagName() == "pass_ticket")
        {
            m_cookieParam.pass_ticket = element.text();
        }
        else
        {
//            ok = lse;
        }
    }
    return ok;
}

void ZLoginWidget::onFinished(const ZRequestAction::ZRequestResponse &response)
{
    LOG_FUNCTION;
    if(response.code != 0)
    {
        LOG_TEST(QString("request is error").arg(QString(response.data)));
        return;
    }
    switch (response.type)
    {
    case HttpRequestType::TYPE_REQUEST_LOGIN_UUID:
    {
        m_uuid = parseUuid(response.data);
        requestQrCode(m_uuid);
        break;
    }
    case HttpRequestType::TYPE_REQUEST_QR_CODE:
    {
        QImage image = QImage::fromData(response.data);
        ui->label->setPixmap(QPixmap::fromImage(image));
        requestWaitLogin(m_uuid,1);
//        requestWaitLogin(m_uuid,0);
        break;
    }
    case HttpRequestType::TYPE_REQUEST_WAIT_LOGIN:
    {
//        requestWaitLogin(m_uuid,0);
        QString res = response.data;
        ui->textEdit->setHtml(res);
        if(res.isEmpty())
        {
            if( !m_timer.isActive())
            {
                m_timer.start();
            }
//            requestWaitLogin(m_uuid,0);
        }
        else if(res.contains("window.code=201"))
        {
            m_tip = 0;
            ui->textEdit->append(QString("扫描成功"));
            if(!m_timer.isActive())
            {
                m_timer.start();
            }
        }
        else if(res.contains("window.code=200"))
        {
            ui->textEdit->append(QString("登陆成功"));
            bool ok = parseRedirectUri(response.data);
            if(ok)
            {
                requestCookie(m_uuid,m_ticket,m_scan);
            }
        }

        break;
    }
    case HttpRequestType::TYPE_REQUEST_COOKIE:
    {
        bool ok = parseCookieData(response.data);
        emit sigLoginFinished(ok);
        if(ok)
        {
            this->close();
        }
        else
        {
            ui->textEdit->append("login error");
            ui->stackedWidget->setCurrentIndex(1);
        }
        break;
    }
    default:
        break;
    }
    LOG_TEST(QString(response.data));
}

void ZLoginWidget::on_btnNext_clicked()
{
    int index = ui->stackedWidget->currentIndex() + 1;
    index = index % ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(index);
}

void ZLoginWidget::onRequestWaitLogin()
{
    LOG_FUNCTION;
    requestWaitLogin(m_uuid,m_tip);
}
