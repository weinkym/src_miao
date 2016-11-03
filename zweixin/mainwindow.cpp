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
    connect(action,SIGNAL(sigFinished(ZRequestAction::ZRequestResponse)),this,SLOT(onFinished(ZRequestAction::ZRequestResponse)));
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
        m_userData = Zpublic::parseUserData(userObjMap);
        m_userAvatar.m_url = m_userData.HeadImgUrl;
        m_userAvatar.m_userName = m_userData.NickName;
        LOG_TEST(QString("m_userAvatar.m_userName = %1").arg(m_userAvatar.m_userName));
        LOG_TEST(QString("m_userAvatar.m_url = %1").arg(m_userAvatar.m_url));
        m_userAvatar.requestAvatar();
    }
}

void MainWindow::onFinished(const ZRequestAction::ZRequestResponse &response)
{
    LOG_FUNCTION;
    if(response.code != 0)
    {
        LOG_TEST(QString("request is error").arg(QString(response.data)));
        return;
    }
    switch (response.type)
    {

    case HttpRequestType::TYPE_REQUEST_WX_INIT:
    {
        bool ok = parseInitData(response.data);
        ui->textEdit->append(QString(response.data));
        break;
    }
    default:
        break;
    }
    LOG_TEST(QString(response.data));
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
