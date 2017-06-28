#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zloginwidget.h"
#include "zpublicaction.h"
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_loginWidget = NULL;
    m_loginWidget = new ZLoginWidget();
    connect(m_loginWidget,SIGNAL(sigLoginFinished(bool)),this,SLOT(onLoginFinished(bool)));
    connect(&m_userAvatar,SIGNAL(sigUpdate()),this,SLOT(onTest()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLogin()
{
    m_loginWidget->show();
    m_loginWidget->requestUuid();
}

void MainWindow::requestInit()
{
    ZPublicAction *action = ZPublicAction::createWxInitAction(m_baseRequestParam);
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
    action->trigger();
}

void MainWindow::requestStatusNotify()
{
    LOG_FUNCTION;
    ZPublicAction *action = ZPublicAction::createStatusNotify(m_baseRequestParam,m_userData.UserName,m_userData.UserName);
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
    action->trigger();
}

void MainWindow::requestContact()
{
    LOG_FUNCTION;
    ZPublicAction *action = ZPublicAction::createGetContact(m_baseRequestParam);
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
    action->trigger();
}

bool MainWindow::parseInitData(const QByteArray &byteArray)
{
    QJsonParseError errorString;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&errorString);
    LOG_TEST(QString("errorString = %1").arg(errorString.errorString()));
    QVariantMap objMap = doc.toVariant().toMap();
    LOG_TEST(QString("objMap.keys()=%1").arg(QStringList(objMap.keys()).join("-")));
    QString key = "User";
    if(objMap.contains(key))
    {
        QVariantMap userObjMap = objMap.value(key).toMap();
        LOG_TEST(QString("userObjMap.keys()=%1").arg(QStringList(userObjMap.keys()).join("-")));
        m_userData = Z_WX_USER_DATA::parseMap(userObjMap);
        m_userAvatar.m_url = m_userData.HeadImgUrl;
        m_userAvatar.m_userName = m_userData.NickName;
        LOG_TEST(QString("m_userAvatar.m_userName = %1").arg(m_userAvatar.m_userName));
        LOG_TEST(QString("m_userAvatar.m_url = %1").arg(m_userAvatar.m_url));
        m_userAvatar.requestAvatar();
    }
    key = "SyncKey";
    if(objMap.contains(key))
    {
        m_syncKeyList = Z_WX_SyncKeyList::parseList(objMap.value(key).toMap().value("List").toList());
        LOG_TEST(QString("m_syncKeyList.itemList.count = %1").arg(m_syncKeyList.itemList.count()));
    }
    return true;
}

bool MainWindow::parseContactData(const QByteArray &byteArray)
{
    LOG_FUNCTION;
    QJsonParseError errorString;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&errorString);
    LOG_TEST(QString("errorString = %1").arg(errorString.errorString()));
    QVariantMap objMap = doc.toVariant().toMap();
    LOG_TEST(QString("objMap.keys()=%1").arg(QStringList(objMap.keys()).join("-")));
    QVariantList objList = objMap.value("MemberList").toList();
    for(auto obj:objList)
    {
        Z_WX_USER_DATA d = Z_WX_USER_DATA::parseMap(obj.toMap());
        //"ContactFlag": 1-好友， 2-群组， 3-公众号
        if(!m_contackMap.contains(d.UserName) && d.ContactFlag == 1)
        {
            Z_WX_USER_DATA *pObj = new Z_WX_USER_DATA;
            *pObj = d;
            m_contackMap.insert(d.UserName,QSharedPointer<Z_WX_USER_DATA>(pObj));
            LOG_TEST(QString("UserName = %1,NickName = %2").arg(pObj->UserName).arg(pObj->NickName));
        }
    }
    LOG_TEST(QString("m_contackMap.count = %1").arg(m_contackMap.count()));
    return true;
}

void MainWindow::onRequestFinished(const CPB::RequestReplyData &response)
{
    LOG_FUNCTION;
    if(response.statusCode > 200)
    {
        LOG_TEST(QString("request is error").arg(QString(response.replyData)));
        return;
    }
    switch (response.type)
    {

    case HttpRequestType::TYPE_REQUEST_WX_INIT:
    {
        bool ok = parseInitData(response.replyData);
        if(ok)
        {
            requestStatusNotify();
        }
        break;
    }
    case TYPE_REQUEST_STATUS_NOTIFY:
    {
        requestContact();
        break;
    }
    case TYPE_REQUEST_CONTACT:
    {
        bool ok = parseContactData(response.replyData);
        break;
    }
    default:
        break;
    }
    LOG_TEST(QString(response.replyData));
}

void MainWindow::on_btnTest01_clicked()
{
    //
}

void MainWindow::onLoginFinished(bool ok)
{
    LOG_FUNCTION;
    this->show();
    if(!ok)
    {
        return;
    }
    Z_WX_COOKIE_PARAM param = m_loginWidget->getCookieParam();
    QString str15("e");
    for(int i = 0; i < 15;++i)
    {
        str15.append(QString::number(qrand()%10));
    }
    m_baseRequestParam.deviceID = str15;
    m_baseRequestParam.skey = param.skey;
    m_baseRequestParam.sid = param.wxsid;
    m_baseRequestParam.uin = param.wxuin;
    m_baseRequestParam.pass_ticket = param.pass_ticket;
    requestInit();
}

void MainWindow::onTest()
{
    LOG_FUNCTION;
    ui->labelAvatar->setPixmap(QPixmap::fromImage(m_userAvatar.m_image));
}
