#ifndef ZWJSBRIDGEOBJECT_H
#define ZWJSBRIDGEOBJECT_H
#include <QObject>
#include <QWebEngineView>
#include "zwydwpublic.h"

class ZWJSBridgeObject : public QObject
{
    Q_OBJECT

public:
    enum DateUpdateType
    {
        DATE_UPDATE_TYPE_MONEY,
    };

    enum Status
    {
        STATUS_UNDEFINE,
        STATUS_INDEX_LOADING,
        STATUS_INDEX_FINISHED,
        STATUS_LOGIN_FINISHED,
    };


    ZWJSBridgeObject(QObject *parent = Q_NULLPTR);
    ~ZWJSBridgeObject();
    QWebEngineView *getView();
    void runJavaScript(const QString &js);
    void load(const QUrl &url);
    void start();

private:
    void runJS();
    QString getStatusJS();
    QString getStatusKey();
    void initBaseJS();
    QString getJSFileData(const QString &filePath);

    QDate getAmountDate(const QString &src);
    QStringList getMatchList(const QString &src,const QRegExp &re);
    double conertStringToDouble(const QString &src,bool &ok);

    void doResultLoginFinished(const QVariantMap &dataMap);

public slots:
    void onRepaymentPageCount(int count);
    void onAppendRepaymentRecord(const QString &title,const QString &code,const QString &status,const QString &rate,const QString &dRate,const QString &money,
                                 const QString &repaymentMoney,const QString &interimRepaymentMoney,const QString &deadline,const QString &repaymentDate);

    void doTest(const QString &text,int type);

    void onLoadFinished(bool finished);

    void onViewDestroyed(QObject *obj);

    void onError(const QString &error,int type,int needReload);
    void onWarning(const QString &error,int type);
    void onDebug(const QString &error,int type);

    void onAmountCallback(const QString &receivable_amount,const QString &receivable_amount_date,
                          const QString &refund_amount,const QString &refund_amount_date);

    void onJSResultCallabk(const QString &jsonData,int type);

signals:
    void sigTest(const QString &text,int type);
    void sigPageCountChanged(int count);
    void sigAmountChanged();
    void sigErrorHappend(const QString &errorString);
    void sigDataChanged(int type);

public:
    QWebEngineView *m_view;
    Status m_status;

    int m_totalPageCount;
    int m_currentPageIndex;
    int m_crrentPageCount;
    int m_currentIgnoreErrorTimes = 0;
    int m_maxIgnoreErrorTimes = 3;
    bool m_isLoading;
    ZWYDW::MoneyData m_moneyData;
    QString m_userName;
    QString m_password;
    QString m_outDirPath;
    QString m_webchannelJSString;
    QString m_publicJSString;
    QStringList m_jsList;
};

#endif // ZWJSBRIDGEOBJECT_H
