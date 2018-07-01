#include "zwjsbridgeobject.h"
#include "zwlog.h"
#include <QFile>
#include <QDateTime>
#include <QWebChannel>
#include <QTimer>

ZWJSBridgeObject::ZWJSBridgeObject(QObject *parent)
    :QObject(parent)
    ,m_view(new QWebEngineView(NULL))
{
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("ZWJSBridgeObject", this);
    m_view->page()->setWebChannel(channel);

    connect(m_view,SIGNAL(loadFinished(bool)),this,SLOT(onLoadFinished(bool)));
    connect(m_view,SIGNAL(destroyed(QObject*)),this,SLOT(onViewDestroyed(QObject*)));
}

ZWJSBridgeObject::~ZWJSBridgeObject()
{

}

void ZWJSBridgeObject::runJS()
{
    ZW_LOG_FUNCTION;

    ZW_VALUE_LOG_INFO(m_status);
    int msecs = qrand() % 5000 + 1000;
    QTimer::singleShot(msecs,[this]{
        ZW_VALUE_LOG_INFO(m_status);
        QString jsString = getStatusJS();
//        ZW_VALUE_LOG_INFO(jsString);
        if(m_view)
        {
            m_view->page()->runJavaScript(jsString);
        }
    });
}

QString ZWJSBridgeObject::getStatusJS()
{
    QString jsString;
    QString filePath;
    switch (m_status) {
    case STATUS_INDEX:
    {
        jsString.append("var demo = document.getElementById('login_button');\n");
        jsString.append(QString("document.getElementById('keywords').value=%1;\n").arg(m_userName));
        jsString.append(QString("document.getElementById('password').value='%1';\n").arg(m_password));
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

void ZWJSBridgeObject::initWebengineJS()
{
    QFile file(":/res/qwebchannel.js");
    if(file.open(QIODevice::ReadOnly))
    {
        m_webchannelJSString = QString(file.readAll());
        file.close();
    }
}

void ZWJSBridgeObject::onRepaymentPageCount(int count)
{
    m_totalPageCount = count;
}

void ZWJSBridgeObject::onAppendRepaymentRecord(const QString &title, const QString &code, const QString &status,
                                               const QString &rate, const QString &dRate, const QString &money,
                                               const QString &repaymentMoney, const QString &interimRepaymentMoney,
                                               const QString &deadline, const QString &repaymentDate)
{
    //
}

void ZWJSBridgeObject::doTest(const QString &text, int type)
{
    ZW_LOG_INFO(QString("text=%1,type = 2").arg(text).arg(type));
}

void ZWJSBridgeObject::onLoadFinished(bool finished)
{
    ZW_LOG_DEBUG("TTTTTT");
    if(m_view)
    {
        static int s_index = 0;
        m_view->page()->toHtml([this](QString html)
        {
            QFile file(QString("/Users/miaozw/Documents/TEMP/ydw/%1.html").arg(s_index++));
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

void ZWJSBridgeObject::onViewDestroyed(QObject *obj)
{
    m_view = NULL;
}

void ZWJSBridgeObject::onError(const QString &error, int type)
{
    ZW_VALUE_LOG_INFO(error);
    ZW_VALUE_LOG_INFO(type);
}
