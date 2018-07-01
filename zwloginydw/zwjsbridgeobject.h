#ifndef ZWJSBRIDGEOBJECT_H
#define ZWJSBRIDGEOBJECT_H
#include <QObject>
#include <QWebEngineView>

class ZWJSBridgeObject : public QObject
{
    Q_OBJECT

public:
    enum Status
    {
        STATUS_UNDEFINE,
        STATUS_INDEX,
        STATUS_LOGINING,
        STATUS_MAINPAGE,
        STATUS_PAGE_LIST,
        STATUS_LOAD_PAGE,
        STATUS_READ_PAGE,
        STATUS_LOAD_APPLYING,
    };


    ZWJSBridgeObject(QObject *parent = Q_NULLPTR);
    ~ZWJSBridgeObject();

private:
    void runJS();
    QString getStatusJS();
    void initWebengineJS();

public slots:
    void onRepaymentPageCount(int count);
    void onAppendRepaymentRecord(const QString &title,const QString &code,const QString &status,const QString &rate,const QString &dRate,const QString &money,
    const QString &repaymentMoney,const QString &interimRepaymentMoney,const QString &deadline,const QString &repaymentDate);
//    void onLoadApplyingFinished();
//    void onApplyingFinished();

    void doTest(const QString &text,int type);

    void onLoadFinished(bool finished);

    void onViewDestroyed(QObject *obj);

    void onError(const QString &error,int type);

signals:
    void sigTest(const QString &text,int type);
    void sigPageCountChanged(int count);

private:
    QWebEngineView *m_view;

    int m_totalPageCount;
    int m_currentPageIndex;
    int m_crrentPageCount;
    QString m_userName;
    QString m_password;
    QString m_outDirPath;
    Status m_status;
    QString m_webchannelJSString;
    QStringList m_jsList;

};

#endif // ZWJSBRIDGEOBJECT_H
