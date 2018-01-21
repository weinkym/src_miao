#ifndef ZWJSBRIDGEOBJECT_H
#define ZWJSBRIDGEOBJECT_H
#include <QObject>

class ZWJSBridgeObject : public QObject
{
    Q_OBJECT

public:
    ZWJSBridgeObject(QObject *parent = Q_NULLPTR);
    ~ZWJSBridgeObject();

public slots:
    void doTest(const QString &text,int type);
    void test();
    void pageCount(int count);

signals:
    void sigTest(const QString &text,int type);
    void sigPageCountChanged(int count);

private:

};

#endif // ZWJSBRIDGEOBJECT_H
