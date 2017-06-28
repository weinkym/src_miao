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
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
    action->trigger();
}

void ZLoginWidget::requestQrCode(const QString &uuid)
{
    LOG_FUNCTION;
    ZRequestAction *action = ZPublicAction::createQrCodeAction(uuid);
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
    action->trigger();
}

void ZLoginWidget::requestWaitLogin(const QString &uuid, int tip)
{
    LOG_FUNCTION;
    ZRequestAction *action = ZPublicAction::createWaitLoginAction(uuid,tip);
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
    action->trigger();
}

void ZLoginWidget::requestCookie(const QString &uuid, const QString &ticket, const QString &scan)
{
    LOG_FUNCTION;
    ZRequestAction *action = ZPublicAction::createCookieAction(uuid,ticket,scan);
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
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

void ZLoginWidget::onRequestFinished(const CPB::RequestReplyData &response)
{
    LOG_FUNCTION;
    if(response.statusCode > 200)
    {
        LOG_TEST(QString("request is error").arg(QString(response.replyData)));

        if(response.statusCode == 408 && response.type == TYPE_REQUEST_WAIT_LOGIN)
        {
            requestUuid();
        }
        return;
    }
    switch (response.type)
    {
    case HttpRequestType::TYPE_REQUEST_LOGIN_UUID:
    {
        m_uuid = parseUuid(response.replyData);
        requestQrCode(m_uuid);
        break;
    }
    case HttpRequestType::TYPE_REQUEST_QR_CODE:
    {
        QImage image = QImage::fromData(response.replyData);


        ui->labelImage->setPixmap(QPixmap::fromImage(image).scaled(ui->labelImage->size()));
        requestWaitLogin(m_uuid,1);
//        requestWaitLogin(m_uuid,0);
        break;
    }
    case HttpRequestType::TYPE_REQUEST_WAIT_LOGIN:
    {
        QString res = response.replyData;
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
            ui->labelStatus->setText(QString("扫描成功"));
            if(!m_timer.isActive())
            {
                m_timer.start();
            }
        }
        else if(res.contains("window.code=200"))
        {
            ui->labelStatus->setText(QString("登陆成功"));
            bool ok = parseRedirectUri(response.replyData);
            if(ok)
            {
                requestCookie(m_uuid,m_ticket,m_scan);
            }
            else
            {
                ui->labelStatus->setText(QString("解析错误"));
            }
        }

        break;
    }
    case HttpRequestType::TYPE_REQUEST_COOKIE:
    {
        bool ok = parseCookieData(response.replyData);
        emit sigLoginFinished(ok);
        if(ok)
        {
            this->close();
        }
        else
        {
            ui->labelStatus->setText("login error");
        }
        break;
    }
    default:
        break;
    }
    LOG_TEST(QString(response.replyData));
}

void ZLoginWidget::onRequestWaitLogin()
{
    LOG_FUNCTION;
    requestWaitLogin(m_uuid,m_tip);
}
