#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zloginwidget.h"
#include "zpublicaction.h"
#include <QJsonDocument>
#include "zcontackitemwidget.h"
#include "ccontactmanager.h"
#include "cloginmanager.h"
#include "zinputmessagedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_loginWidget = NULL;
    m_loginWidget = new ZLoginWidget();
    connect(m_loginWidget,SIGNAL(sigLoginFinished(bool)),this,SLOT(onLoginFinished(bool)));
//    connect(&m_userAvatar,SIGNAL(sigUpdate()),this,SLOT(onTest()));
    CContactManager *mgr = CContactManager::getInstance();
    connect(mgr,SIGNAL(sigDateUpdate(int,QVariant)),this,SLOT(onDateUpdate(int,QVariant)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLogin()
{
    m_loginWidget->show();
    CLoginManager::getInstance()->start();

//    m_loginWidget->requestUuid();
}

void MainWindow::requestInit()
{
//    ZPublicAction *action = ZPublicAction::createWxInitAction(m_baseRequestParam);
//    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
//    action->trigger();
}

void MainWindow::requestStatusNotify()
{
    ZW_LOG_FUNCTION;
//    ZPublicAction *action = ZPublicAction::createStatusNotify(m_baseRequestParam,m_userData.UserName,m_userData.UserName);
//    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
//    action->trigger();
}

void MainWindow::requestContact()
{
    ZW_LOG_FUNCTION;
//    ZPublicAction *action = ZPublicAction::createGetContact(m_baseRequestParam);
//    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
//    action->trigger();
}

void MainWindow::requestGrounp()
{
    //
}


bool MainWindow::parseInitData(const QByteArray &byteArray)
{
//    QJsonParseError errorString;
//    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&errorString);
//    LOG_TEST(QString("errorString = %1").arg(errorString.errorString()));
//    QVariantMap objMap = doc.toVariant().toMap();
//    LOG_TEST(QString("objMap.keys()=%1").arg(QStringList(objMap.keys()).join("-")));
//    QString key = "User";
//    if(objMap.contains(key))
//    {
//        QVariantMap userObjMap = objMap.value(key).toMap();
//        LOG_TEST(QString("userObjMap.keys()=%1").arg(QStringList(userObjMap.keys()).join("-")));
//        m_userData = Z_WX_USER_DATA::parseMap(userObjMap);
//        m_userAvatar.m_url = m_userData.HeadImgUrl;
//        m_userAvatar.m_userName = m_userData.NickName;
//        LOG_TEST(QString("m_userAvatar.m_userName = %1").arg(m_userAvatar.m_userName));
//        LOG_TEST(QString("m_userAvatar.m_url = %1").arg(m_userAvatar.m_url));
//        m_userAvatar.requestAvatar();
//    }
//    key = "SyncKey";
//    if(objMap.contains(key))
//    {
//        m_syncKeyList = Z_WX_SyncKeyList::parseList(objMap.value(key).toMap().value("List").toList());
//        LOG_TEST(QString("m_syncKeyList.itemList.count = %1").arg(m_syncKeyList.itemList.count()));
//    }
//    key = "ContactList";
//    if(objMap.contains(key))
//    {
//        QVariantList objList = objMap.value(key).toList();
//        for(auto d:objList)
//        {
//            Z_WX_USER_DATA obj = Z_WX_USER_DATA::parseMap(d.toMap());
//            if(obj.UserName.startsWith("@@"))
//            {
//                m_groupNameList.append(obj.UserName);
//            }
//        }
//        LOG_TEST(QString("m_syncKeyList.itemList.count = %1").arg(m_syncKeyList.itemList.count()));
//    }
//    return true;
}


void MainWindow::onRequestFinished(const CPB::RequestReplyData &response)
{
    ZW_LOG_FUNCTION;
//    if(response.statusCode > 200)
//    {
//        LOG_TEST(QString("request is error").arg(QString(response.replyData)));
//        return;
//    }
//    switch (response.type)
//    {

//    case HttpRequestType::TYPE_REQUEST_WX_INIT:
//    {
//        bool ok = parseInitData(response.replyData);
//        if(ok)
//        {
//            requestStatusNotify();
//        }
//        break;
//    }
//    case TYPE_REQUEST_STATUS_NOTIFY:
//    {
//        CContactManager::getInstance()->requestContact(m_baseRequestParam);
//        CContactManager::getInstance()->requestContactGroup(m_baseRequestParam,m_groupNameList);
//        break;
//    }
//    default:
//        break;
//    }
//    LOG_TEST(QString(response.replyData));
}

void MainWindow::on_btnTest01_clicked()
{

    onTest();
    return;
    QListWidgetItem *item = ui->listWidgetContact->currentItem();
    ZContackItemWidget *itemWidget = qobject_cast<ZContackItemWidget*>(ui->listWidgetContact->itemWidget(item));
    if(itemWidget == NULL)
    {
        return;
    }
    QSharedPointer<Z_WX_USER_DATA> toUser = itemWidget->getContact();
    if(toUser.isNull())
    {
        return;
    }

    CContactManager::getInstance()->sendMessage(toUser.data()->UserName,ui->textEditChat->toPlainText());
//    ZPublicAction *action = ZPublicAction::createSendMessage(m_baseRequestParam,m_userData.UserName,toUser.data()->UserName,ui->textEditChat->toPlainText());
    ui->textEditChat->clear();
//    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
//    action->trigger();
}

void MainWindow::onLoginFinished(bool ok)
{
    ZW_LOG_FUNCTION;
    this->show();
    if(!ok)
    {
        return;
    }
//    Z_WX_COOKIE_PARAM param = m_loginWidget->getCookieParam();
//    QString str15("e");
//    for(int i = 0; i < 15;++i)
//    {
//        str15.append(QString::number(qrand()%10));
//    }
//    m_baseRequestParam.deviceID = str15;
//    m_baseRequestParam.skey = param.skey;
//    m_baseRequestParam.sid = param.wxsid;
//    m_baseRequestParam.uin = param.wxuin;
//    m_baseRequestParam.pass_ticket = param.pass_ticket;
//    requestInit();
}

void MainWindow::onTest()
{
    ZW_LOG_FUNCTION;
    ZInputMessageDialog dlg;
    dlg.resetContacts(CContactManager::getInstance()->m_contackMap.values());
    dlg.exec();
//    ui->labelAvatar->setPixmap(QPixmap::fromImage(m_userAvatar.m_image));
}

void MainWindow::onDateUpdate(int type, const QVariant &value)
{
    switch (type)
    {
    case CPB::DATA_UPDATE_TYPE_CONTACT_LIST:
    {
        ui->listWidgetContact->clear();
        QSize size(200,50);
        QMapIterator<QString,QSharedPointer<Z_WX_USER_DATA> >iter(CContactManager::getInstance()->m_contackMap);
        while(iter.hasNext())
        {
            iter.next();
//            if(!iter.key().startsWith("@@"))
//            {
//                continue;
//            }
            QListWidgetItem *item = new QListWidgetItem;
            ZContackItemWidget *itemWidget = new ZContackItemWidget(iter.value());
            itemWidget->setMinimumSize(size);
            itemWidget->setMaximumSize(size);
            item->setSizeHint(itemWidget->size());
            ui->listWidgetContact->addItem(item);
            ui->listWidgetContact->setItemWidget(item,itemWidget);
        }
        break;
    }
    case CPB::DATA_UPDATE_TYPE_GROUP_LIST:
    {
        ui->listWidgetGroup->clear();
        QSize size(200,50);
        QMapIterator<QString,QSharedPointer<Z_WX_USER_DATA> >iter(CContactManager::getInstance()->m_groupMap);
        while(iter.hasNext())
        {
            iter.next();

            QListWidgetItem *item = new QListWidgetItem;
            ZContackItemWidget *itemWidget = new ZContackItemWidget(iter.value());
            itemWidget->setMinimumSize(size);
            itemWidget->setMaximumSize(size);
            item->setSizeHint(itemWidget->size());
            ui->listWidgetGroup->addItem(item);
            ui->listWidgetGroup->setItemWidget(item,itemWidget);
        }
        break;
    }
    default:
        break;
    }
}
