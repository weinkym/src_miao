#include "zwwebengineview.h"
#include "zwlog.h"
#include <QFile>
#include <QWebChannel>

#include "zwjsbridgeobject.h"

ZWWebengineView::ZWWebengineView(QWidget *parent)
    :QWebEngineView(parent)
    ,m_bridgeObject(NULL)
{
    connect(this,SIGNAL(loadFinished(bool)),this,SLOT(onLoadFinished(bool)));
    QString key = "//========//";
    QFile file(":/res/all.js");
    if(file.open(QIODevice::ReadOnly))
    {
        QString jsString;
        while(!file.atEnd())
        {
            QString lineString(file.readLine());
            if(lineString.startsWith(key))
            {
//                jsString.append(lineString);
                if(!jsString.isEmpty())
                {
                    m_jsList.append(jsString);
                }
                jsString = "";
            }
            else if(lineString.startsWith("//") || lineString == "\n")
            {
                continue;
            }
            else
            {
                jsString.append(lineString);
                jsString.append("\n");
            }
//            ZW_LOG_INFO(jsString);
        }
        file.close();
    }
    else
    {
        ZW_LOG_CRITICAL("open file is error");
    }

    for(auto obj:m_jsList)
    {
//        ZW_LOG_DEBUG(obj);
//        ZW_LOG_DEBUG("===============================");
    }

//    this->load(QUrl("https://www.baidu.com/"));
    QWebChannel *channel = new QWebChannel(this);
    m_bridgeObject = new ZWJSBridgeObject(this);
    channel->registerObject(QStringLiteral("bridge"), m_bridgeObject);

//    connect(m_bridgeObject, SIGNAL(sigKickedByOtherClient(QString)), this, SLOT(onKickedByOtherDevice(QString)));
//    connect(m_bridgeObject, SIGNAL(sigSpeakingStatusChanged(int)), this, SLOT(onSpeakingStatusChanged(int)));
//    connect(m_bridgeObject, SIGNAL(sigChangeStream(int)), this, SLOT(onChangeStream(int)));
//    connect(m_bridgeObject, SIGNAL(sigChangeRoomStatus(int)), this, SLOT(onChangeRoomStatus(int)));
    this->page()->setWebChannel(channel);
    this->page()->load(QUrl("file:///Users/miaozw/Documents/TEMP/test.htm"));

}

ZWWebengineView::~ZWWebengineView()
{

}

void ZWWebengineView::startLogin()
{
    m_crrentIndex = 0;
//    this->load(QUrl("http://www.yidai.com/user/login"));
//    this->load(QUrl("https://www.baidu.com/"));
}

void ZWWebengineView::onLoadFinished(bool finished)
{
    ZW_LOG_DEBUG("TTTTTT");
    {
        this->page()->toHtml([this](QString html)
                          {
                              qDebug()<<html;
                          });
    }
    if(m_bridgeObject == NULL)
    {
//        QWebChannel *channel = new QWebChannel(this);

//        m_bridgeObject = new ZWJSBridgeObject(this);
    //    connect(m_bridgeObject, SIGNAL(sigKickedByOtherClient(QString)), this, SLOT(onKickedByOtherDevice(QString)));
    //    connect(m_bridgeObject, SIGNAL(sigSpeakingStatusChanged(int)), this, SLOT(onSpeakingStatusChanged(int)));
    //    connect(m_bridgeObject, SIGNAL(sigChangeStream(int)), this, SLOT(onChangeStream(int)));
    //    connect(m_bridgeObject, SIGNAL(sigChangeRoomStatus(int)), this, SLOT(onChangeRoomStatus(int)));
//        channel->registerObject("bridge", (QObject*)m_bridgeObject);
//        this->page()->setWebChannel(channel);

    }
    return;
    if(finished || m_crrentIndex < m_jsList.count())
    {
        if(m_crrentIndex == 0)
        {
            this->page()->runJavaScript(m_jsList.first());
        }
    }
}

void ZWWebengineView::test()
{

}
