#include "zwjsbridgeobject.h"
#include "zwlog.h"
#include <QFile>
#include <QDateTime>
#include <QWebChannel>
#include <QTimer>
#include <QRegExp>

ZWJSBridgeObject::ZWJSBridgeObject(QObject *parent)
    :QObject(parent)
    ,m_view(new QWebEngineView(NULL))
    ,m_status(STATUS_UNDEFINE)
{
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("ZWJSBridgeObject", this);
    m_view->page()->setWebChannel(channel);

    connect(m_view,SIGNAL(loadFinished(bool)),this,SLOT(onLoadFinished(bool)));
    connect(m_view,SIGNAL(destroyed(QObject*)),this,SLOT(onViewDestroyed(QObject*)));

    initBaseJS();
    m_isLoading = false;
}

ZWJSBridgeObject::~ZWJSBridgeObject()
{

}

QWebEngineView *ZWJSBridgeObject::getView()
{
    return m_view;
}

void ZWJSBridgeObject::runJavaScript(const QString &js)
{
    ZW_LOG_FUNCTION;
//    ZW_VALUE_LOG_INFO(js);
    if(m_view)
    {
        QString jsString;
        jsString.append("function doFun(){\n");
        jsString.append("var obj = new QWebChannel(qt.webChannelTransport, function(channel) {\n");
        jsString.append("var js_callback_object = channel.objects.ZWJSBridgeObject;\n");
        //                jsString.append("js_callback_object.onError(\"adsfadsf\",0);\n");
        jsString.append(QString("var g_type=%1;\n").arg(m_status));
        jsString.append("\n");
        jsString.append(m_publicJSString);
        jsString.append("\n");

        jsString.append(js);
        jsString.append("\n");
        jsString.append("});\n");
        jsString.append("}\n");
        jsString.append("doFun();\n");
//        ZW_VALUE_LOG_INFO(jsString);
        m_view->page()->runJavaScript(m_webchannelJSString+jsString);
    }
}

void ZWJSBridgeObject::load(const QUrl &url)
{
    if(m_view)
    {
        m_view->load(url);
    }
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

void ZWJSBridgeObject::initBaseJS()
{
    m_webchannelJSString = getJSFileData(":/res/qwebchannel.js");
    m_publicJSString = getJSFileData(":/res/js/public.js");
}

QString ZWJSBridgeObject::getJSFileData(const QString &filePath)
{
    QString res;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        res = QString(file.readAll());
        file.close();
    }
    return res;
}

QDate ZWJSBridgeObject::getAmountDate(const QString &src)
{
    QRegExp re("\\d{4}-\\d{2}-\\d{2}");
    QStringList dateList = getMatchList(src,re);
    ZW_VALUE_LOG_INFO(dateList.join(";"));
    if(dateList.isEmpty())
    {
        return QDate();
    }
    return QDate::fromString(dateList.first(),"yyyy-MM-dd");
}

QStringList ZWJSBridgeObject::getMatchList(const QString &src, const QRegExp &re)
{
//    QRegExp tempRx("src=\"([^\"]*)\"");
    int tempPos = 0;
    QStringList tempCapList;
    while ((tempPos = re.indexIn(src, tempPos)) != -1)
    {
        QString cap = re.cap(0);
        tempCapList << cap;
        tempPos += re.matchedLength();
    }
    return tempCapList;
}

double ZWJSBridgeObject::conertStringToDouble(const QString &src, bool &ok)
{
    QString temp = src;
    temp.remove(",");
    return temp.toDouble(&ok);
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
    if(!m_isLoading)
    {
        return;
    }
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

void ZWJSBridgeObject::onWarning(const QString &error, int type)
{
    ZW_VALUE_LOG_INFO(error);
    ZW_VALUE_LOG_INFO(type);
}

void ZWJSBridgeObject::onDebug(const QString &error, int type)
{
    ZW_VALUE_LOG_INFO(error);
    ZW_VALUE_LOG_INFO(type);
}

void ZWJSBridgeObject::onAmountCallback(const QString &receivable_amount, const QString &receivable_amount_date,
                                        const QString &refund_amount, const QString &refund_amount_date)
{
    ZW_VALUE_LOG_INFO(receivable_amount);
    ZW_VALUE_LOG_INFO(receivable_amount_date);
    ZW_VALUE_LOG_INFO(refund_amount);
    ZW_VALUE_LOG_INFO(refund_amount_date);
    QDate receivableDate = getAmountDate(receivable_amount_date);
    QDate refundDate = getAmountDate(refund_amount_date);

    bool ok = false;
    double receivableAmount = conertStringToDouble(receivable_amount,ok);
    double refundAmount = conertStringToDouble(refund_amount,ok);
    ZW_VALUE_LOG_INFO(receivableAmount);
    ZW_VALUE_LOG_INFO(refundAmount);

    ZW_LOG_INFO(receivableDate.toString("SSSS===yyyy-MM-dd"));
    ZW_LOG_INFO(refundDate.toString("SSSS===yyyy-MM-dd"));
}
