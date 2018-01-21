#include "zwwebengineview.h"
#include "zwlog.h"
#include <QFile>
#include <QWebChannel>
#include <QTimer>

#include "zwjsbridgeobject.h"
#define ZW_JS_OBJECT_NAME "ZWWebengineView"

ZWWebengineView::ZWWebengineView(QWidget *parent)
    :QWebEngineView(parent)
    ,m_bridgeObject(NULL)
    ,m_status(STATUS_UNDEFINE)
{
    connect(this,SIGNAL(loadFinished(bool)),this,SLOT(onLoadFinished(bool)));
    QWebChannel *channel = new QWebChannel(this);
//    m_bridgeObject = new ZWJSBridgeObject(this);
    channel->registerObject(ZW_JS_OBJECT_NAME, this);
    this->page()->setWebChannel(channel);

    initWebengineJS();
//    connect(m_bridgeObject, SIGNAL(sigKickedByOtherClient(QString)), this, SLOT(onKickedByOtherDevice(QString)));
//    connect(m_bridgeObject, SIGNAL(sigSpeakingStatusChanged(int)), this, SLOT(onSpeakingStatusChanged(int)));
//    connect(m_bridgeObject, SIGNAL(sigChangeStream(int)), this, SLOT(onChangeStream(int)));
//    connect(m_bridgeObject, SIGNAL(sigChangeRoomStatus(int)), this, SLOT(onChangeRoomStatus(int)));
//    this->page()->load(QUrl("file:///Users/miaozw/Documents/TEMP/test.htm"));
//    this->load(QUrl("https://www.baidu.com/"));

}

ZWWebengineView::~ZWWebengineView()
{

}

void ZWWebengineView::startLogin()
{
    m_crrentPageCount = 0;
    this->load(QUrl("http://www.yidai.com/user/login"));
    //    this->load(QUrl("https://www.baidu.com/"));
}

void ZWWebengineView::setCount(int count)
{
    ZW_LOG_INFO(QString("count=%1").arg(count));
    m_totalPageCount = count;
    m_currentPageIndex = count;
    m_status = STATUS_LOAD_PAGE;
    runJS();
}

void ZWWebengineView::appendText(const QString &text, int count)
{
    ZW_LOG_INFO(QString("text=%1").arg(text));
    ZW_LOG_INFO(QString("count=%1").arg(count));
    QFile file("/Users/miaozw/Documents/TEMP/out.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.write(text.toLocal8Bit().data());
        file.close();
    }
    if(m_currentPageIndex > 1)
    {
        m_status = STATUS_LOAD_PAGE;
        m_currentPageIndex--;
        runJS();
    }
    else
    {
        ZW_LOG_INFO("finished");
    }
}

void ZWWebengineView::initJSList()
{
    m_jsList.clear();
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
    ZW_LOG_INFO(QString("jslistcount=%1").arg(m_jsList.count()));
}

void ZWWebengineView::initWebengineJS()
{
    QFile file(":/res/qwebchannel.js");
    if(file.open(QIODevice::ReadOnly))
    {
        m_webchannelJSString = QString(file.readAll());
        file.close();
    }
}

QString ZWWebengineView::getStatusJS()
{
    QString jsString;
    QString filePath;
    switch (m_status) {
    case STATUS_INDEX:
    {
        jsString.append("var demo = document.getElementById('login_button');\n");
        jsString.append("document.getElementById('keywords').value=13616511205;\n");
        jsString.append("document.getElementById('password').value='miao1280';\n");
        jsString.append("demo.click();\n");
       break;
    }
    case STATUS_LOGINING:
    {
        filePath = QString(":/res/STATUS_LOGINING.js");
        break;
    }
    case STATUS_MAINPAGE:
    {
        filePath = QString(":/res/STATUS_MAINPAGE.js");
        break;
    }
    case STATUS_PAGE_LIST:
    {
        jsString.append(m_webchannelJSString);
        filePath = QString(":/res/STATUS_PAGE_LIST.js");
        break;
    }
    case STATUS_LOAD_PAGE:
    {
        jsString.append(QString("page_jump(\"this\",\"%1\")\n").arg(m_currentPageIndex));
        break;
    }
    case STATUS_READ_PAGE:
    {
        jsString.append(m_webchannelJSString);
        filePath = QString(":/res/STATUS_READ_PAGE.js");
        break;
    }
    default:
        break;
    }
    if(!filePath.isEmpty())
    {
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly))
        {
            jsString.append("\n");
            jsString.append(QString(file.readAll()));
            jsString.append("\n");
            file.close();
        }
    }
    if(m_status == STATUS_READ_PAGE)
    {
        jsString.append(QString("zw_read_page(%1);\n").arg(m_currentPageIndex));
    }
    return jsString;
}

void ZWWebengineView::runJS()
{
    ZW_LOG_FUNCTION;
    ZW_VALUE_LOG_INFO(m_status);
    int msecs = qrand() % 5000 + 1000;
    QTimer::singleShot(msecs,[this]{
        ZW_VALUE_LOG_INFO(m_status);
        QString jsString = getStatusJS();
//        ZW_VALUE_LOG_INFO(jsString);
        this->page()->runJavaScript(jsString);
    });
}

void ZWWebengineView::onLoadFinished(bool finished)
{
//    return;
    ZW_LOG_DEBUG("TTTTTT");
    {
        this->page()->toHtml([this](QString html)
        {
            QFile file(QString("/Users/miaozw/Documents/TEMP/ydw/%1.html").arg(m_currentPageIndex));
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(html.toLocal8Bit().data());
                file.close();
            }
        });
    }
    ZW_VALUE_LOG_INFO(m_status);

    switch (m_status)
    {
    case  STATUS_UNDEFINE:
    {
        m_status = STATUS_INDEX;
        runJS();
        break;
    }
    case STATUS_INDEX:
    {
        m_status = STATUS_LOGINING;
        runJS();
        break;
    }
    case STATUS_LOGINING:
    {
        m_status = STATUS_MAINPAGE;
        runJS();
        break;
    }
    case STATUS_MAINPAGE:
    {
        m_status = STATUS_PAGE_LIST;
        runJS();
        break;
    }
    case STATUS_LOAD_PAGE:
    {
        m_status = STATUS_READ_PAGE;
        runJS();
        break;
    }
    default:
        break;
    }

}

void ZWWebengineView::test()
{

}
