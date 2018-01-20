#ifndef ZWWEBENGINEVIEW_H
#define ZWWEBENGINEVIEW_H

#include <QWebEngineView>

class ZWJSBridgeObject;

class ZWWebengineView : public QWebEngineView
{
    Q_OBJECT
public:
    ZWWebengineView(QWidget *parent = Q_NULLPTR);
    ~ZWWebengineView();
    void startLogin();

private slots:
    void onLoadFinished(bool finished);
    void test();

private:
    ZWJSBridgeObject *m_bridgeObject;
    int m_crrentIndex;
    QStringList m_jsList;
};

#endif // ZWWEBENGINEVIEW_H
