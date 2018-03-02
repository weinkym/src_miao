#ifndef ZWWEBENGINEVIEW_H
#define ZWWEBENGINEVIEW_H

#include <QWebEngineView>

class ZWJSBridgeObject;

class ZWWebengineView : public QWebEngineView
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
    };

    ZWWebengineView(QWidget *parent = Q_NULLPTR);
    ~ZWWebengineView();
    void startLogin();
    void startLogin(const QString &userName,const QString &password,const QString &outDirPath);

public slots:
    void setCount(int count);
    void appendText(const QString &text,int count);

private:
    void initJSList();
    void initWebengineJS();
    QString getStatusJS();

    void runJS();

private slots:
    void onLoadFinished(bool finished);
    void test();

private:
    ZWJSBridgeObject *m_bridgeObject;
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

#endif // ZWWEBENGINEVIEW_H
