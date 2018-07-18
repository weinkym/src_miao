#include "zwjsbridgeobject.h"
#include "zwlog.h"
#include <QFile>
#include <QDateTime>
#include <QWebChannel>
#include <QTimer>
#include <QRegExp>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include "zwydwpublic.h"

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

    connect(&m_loadUrlTimer,SIGNAL(timeout()),this,SLOT(onLoadUrlTimeout()));
    m_loadUrlTimer.setInterval(15000);
    m_loadUrlTimer.setSingleShot(true);

    m_hasApplyMoney = false;

    initBaseJS();
    m_isLoading = false;
    m_loginFinished = false;

    m_userName = "13616511205";
    m_password = "miao1280";
    m_payPassword = "zfydw1280";

//    m_userName = "15267080236";
//    m_password = "ydw120zp";
//    m_payPassword = "zf829816";
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
        jsString.append(QString("var g_needReload=0;\n"));
        jsString.append(QString("var g_return_data_obj={};\n"));
        jsString.append(QString("var g_result_obj={};\n"));
        jsString.append(QString("g_result_obj.error=0;\n"));
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
    ZW_LOG_FUNCTION;
    ZW_VALUE_LOG_INFO(url.toString());
    if(m_view)
    {
        m_loadUrlTimer.stop();
        m_view->load(url);
        m_loadUrlTimer.start();
    }
}

void ZWJSBridgeObject::start()
{
    m_status = STATUS_INDEX_LOADING;
    m_isLoading = true;
    load(QUrl("http://www.yidai.com/user/login"));
}

void ZWJSBridgeObject::runJS()
{
    ZW_LOG_FUNCTION;
    ZW_VALUE_LOG_INFO(m_status);
    int msecs = qrand() % 5000 + 1000;
    QTimer::singleShot(msecs,[this]{
        ZW_VALUE_LOG_INFO(m_status);
        QString jsString = getStatusJS();
        ZW_VALUE_LOG_INFO(jsString);
        if(m_view)
        {
            this->runJavaScript(jsString);
//            m_view->page()->runJavaScript(jsString);
        }
    });
}

QString ZWJSBridgeObject::getStatusJS()
{
    QString jsString;
    QString filePath;
    switch (m_status) {
    case STATUS_INDEX_FINISHED:
    {
        jsString = getJSFileData(":/res/js/login.js").arg(m_userName).arg(m_password);
       break;
    }
    case STATUS_LOGIN_FINISHED:
    {
        jsString = getJSFileData(":/res/js/login_finished.js");
        break;
    }
    case STATUS_APPLYING_PAGE_FINISHED:
    {
        jsString = getJSFileData(":/res/js/applying_page_finished.js");
        break;
    }
    case STATUS_APPLY_PAGE_FINISHED:
    {
        QString dateString = getDateString();
        QString borrow_apr = "15";
        jsString = getJSFileData(":/res/js/apply_page_finished.js").arg(dateString).arg(borrow_apr);
        break;
    }
    case STATUS_STOP_APPLYING:
    {
        jsString = getJSFileData(":/res/js/stop_applying.js").arg(m_payPassword);
        break;
    }
    case STATUS_SELECT_APPLY:
    {
        QString dateString = getDateString();
        ZW_VALUE_LOG_INFO(dateString);
        jsString = getJSFileData(":/res/js/select_apply.js").arg(dateString);
        break;
    }

    default:
        break;
    }

    return jsString;
}

QString ZWJSBridgeObject::getStatusKey()
{
    QString key;
    switch (m_status) {
    case STATUS_INDEX_FINISHED:
    {
        key = "STATUS_INDEX_FINISHED";
       break;
    }
    case STATUS_LOGIN_FINISHED:
    {
        key = "STATUS_LOGIN_FINISHED";
        break;
    }
    default:
        break;
    }
    return key;
}

QString ZWJSBridgeObject::getDateString()
{
    //TODO
    QDate date(2018,7,17);
    date = date.addMonths(1);
    return date.toString("MM-dd");
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

void ZWJSBridgeObject::loadApplyingPage()
{
    ZW_LOG_FUNCTION;
    m_status = STATUS_APPLYING_PAGE_LOADING;
    load(QUrl("https://www.yidai.com/borrowesharemy/index/"));
}

void ZWJSBridgeObject::loadApplyPage()
{
    ZW_LOG_FUNCTION;
    m_status = STATUS_APPLY_PAGE_LOADING;
    load(QUrl("https://www.yidai.com/borroweshareapply/index/"));
}

void ZWJSBridgeObject::doResultLoginFinished(const QVariantMap &dataMap)
{
    ZW_LOG_FUNCTION;
    bool ok = true;
    ZWYDW::MoneyData obj = ZWYDWPublic::parseMoneyData(dataMap,ok);
    ZW_VALUE_LOG_INFO(ok);
    if(ok)
    {
        ZW_VALUE_LOG_INFO(obj.amount);
        ZW_VALUE_LOG_INFO(obj.account_balance);
        ZW_VALUE_LOG_INFO(obj.recent_receivable_amount);
        ZW_VALUE_LOG_INFO(obj.recent_refund_amount);
        ZW_VALUE_LOG_INFO(obj.recent_receivable_amount_date.toString("yyyyMMdd"));
        ZW_VALUE_LOG_INFO(obj.recent_refund_amount_date.toString("yyyyMMdd"));
    }
    if(!ok)
    {
        ZW_VALUE_LOG_INFO(obj.errorString);
        emit sigErrorHappend(QString("parse loginfinished data error %1").arg(obj.errorString));
        if(m_loginFinished)
        {
            doRestart();
        }
        return;
    }
    m_moneyData = obj;
    m_loginFinished = true;
    emit sigDataChanged(DATE_UPDATE_TYPE_MONEY);

    loadApplyingPage();
}

void ZWJSBridgeObject::doResultApplyingPageFinished(const QVariantMap &dataMap)
{
    QString key = "apply_count";
    if(dataMap.contains(key))
    {
        bool ok = true;
        int apply_count = dataMap.value(key).toInt(&ok);
        if(ok)
        {
            if(apply_count == 0)
            {
                loadApplyPage();
            }
            else
            {
                QTimer::singleShot(5000,[this]{
                    m_status = STATUS_STOP_APPLYING;
                    runJS();
                });
            }
        }
    }
}

void ZWJSBridgeObject::doRestart()
{
    ZW_LOG_FUNCTION;
    if(m_loginFinished)
    {
        m_loadUrlTimer.stop();
        //
        QTimer::singleShot(5000,[this]{
           loadApplyingPage();
        });
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
    ZW_LOG_FUNCTION;
    ZW_VALUE_LOG_INFO_BOOL(finished);
    ZW_VALUE_LOG_INFO_BOOL(m_isLoading);
    m_loadUrlTimer.stop();
    if(!m_isLoading)
    {
        return;
    }
    ZW_LOG_INFO(QString("onLoadFinished status = %1").arg(m_status));
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

    switch (m_status)
    {
    case  STATUS_INDEX_LOADING:
    {
        m_status = STATUS_INDEX_FINISHED;
        runJS();
        break;
    }

    case STATUS_INDEX_FINISHED:
    {
        m_status = STATUS_LOGIN_FINISHED;
        runJS();
        break;
    }
    case STATUS_APPLYING_PAGE_LOADING:
    {
        m_status = STATUS_APPLYING_PAGE_FINISHED;
        runJS();
        break;
    }
    case STATUS_APPLY_PAGE_LOADING:
    {
        m_status = STATUS_APPLY_PAGE_FINISHED;
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

void ZWJSBridgeObject::onError(const QString &error, int type, int needReload)
{
    ZW_VALUE_LOG_INFO(error);
    ZW_VALUE_LOG_INFO(type);
    ZW_VALUE_LOG_INFO(needReload);
    if(needReload && m_currentIgnoreErrorTimes < m_maxIgnoreErrorTimes)
    {
        m_currentIgnoreErrorTimes++;
        runJS();
    }
    else
    {
        m_currentIgnoreErrorTimes = 0;
        //
    }
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
    m_currentIgnoreErrorTimes = 0;
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

void ZWJSBridgeObject::onJSResultCallabk(const QString &jsonData, int type)
{
    ZW_VALUE_LOG_INFO(jsonData);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toLocal8Bit(), &error);
    if(error.error != QJsonParseError::NoError)
    {
        ZW_VALUE_LOG_INFO(error.errorString());
        return;
    }
    QVariantMap objMap = doc.toVariant().toMap();
    if(objMap.isEmpty())
    {
        ZW_LOG_WARNING("json data is not obj");
        return;
    }
    qDebug()<<objMap;


    QVariantMap dataMap = objMap.value("data").toMap();
    ZW_VALUE_LOG_INFO(QStringList(dataMap.keys()).join(";"));
    if(dataMap.isEmpty())
    {
        ZW_LOG_WARNING("dataMap.isEmpty()");
//        return;
    }
    bool hasError = (objMap.value("error",1).toInt() != 0);
    bool needReload = (objMap.value("needReload",1).toInt() != 0);
    QString errorKey = objMap.value("errorKey").toString();
    ZW_VALUE_LOG_INFO_BOOL(hasError);
    ZW_VALUE_LOG_INFO_BOOL(needReload);
    ZW_VALUE_LOG_INFO(errorKey);

    if(hasError)
    {
        ZW_VALUE_LOG_INFO(errorKey);
        if(needReload && m_currentIgnoreErrorTimes < m_maxIgnoreErrorTimes)
        {
            m_currentIgnoreErrorTimes++;
            runJS();
        }
        else
        {
            m_currentIgnoreErrorTimes = 0;
            ZW_VALUE_LOG_INFO(errorKey);
            emit sigErrorHappend(errorKey);
            doRestart();
            return;
        }
    }

    switch (type)
    {
    case STATUS_LOGIN_FINISHED:
        doResultLoginFinished(dataMap);
        break;
    case STATUS_APPLYING_PAGE_FINISHED:
        doResultApplyingPageFinished(dataMap);
        break;
    case STATUS_STOP_APPLYING:
    {
        m_hasApplyMoney = dataMap.value("has_money",false).toBool();
        ZW_VALUE_LOG_INFO_BOOL(m_hasApplyMoney);
//        int min = (5 + qrand() % 5) * 60;
        int min = (1 + qrand() % 2) * 60;
        min = 30;
        if(m_hasApplyMoney)
        {
            min = 3;
        }
//        min = 10;
        ZW_VALUE_LOG_INFO(min);
        QTimer::singleShot(min * 1000,[this]{
            loadApplyingPage();
        });
        break;
    }
    case STATUS_APPLY_PAGE_FINISHED:
    {
        ZW_LOG_INFO(QString("finished apply js"));
        bool need_select = dataMap.value("need_select",false).toBool();
        ZW_VALUE_LOG_INFO_BOOL(need_select);
        if(need_select)
        {
            QTimer::singleShot(5000,[this]{
                m_status = STATUS_SELECT_APPLY;
                runJS();
            });
            break;
        }
        else
        {
            m_currentTime++;
            if(m_currentTime < 35)
            {
                QTimer::singleShot(1 * 60 * 1000,[this]{
                    loadApplyingPage();
                });
                break;
            }
        }
        break;
    }
    case STATUS_SELECT_APPLY:
    {
        bool select_finished = dataMap.value("select_finished",false).toBool();
        ZW_VALUE_LOG_INFO_BOOL(select_finished);
        if(select_finished)
        {
            m_status = STATUS_APPLY_PAGE_FINISHED;
            runJS();
        }
        break;
    }
    default:
        break;
    }
}

void ZWJSBridgeObject::onLoadUrlTimeout()
{
    ZW_LOG_FUNCTION;
    loadApplyingPage();
}

//void ZWJSBridgeObject::onLastIgnoreErrorKey(int type, const QString &msg)
//{
//    ZW_VALUE_LOG_INFO(type);
//    ZW_VALUE_LOG_INFO(msg);
//    m_lastIgnoreErrorType = key;
//}

//void ZWJSBridgeObject::onErrorStatus()
//{

//}
