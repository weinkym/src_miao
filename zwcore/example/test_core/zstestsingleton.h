#ifndef ZSTESTSINGLETON_H
#define ZSTESTSINGLETON_H
#include "zwsingleton.h"
#include <QObject>


class ZSTestSingleton : public QObject,public ZWSingleton<ZSTestSingleton>
{
    Q_OBJECT

    friend class ZWSingleton<ZSTestSingleton>;
public:
    void doTest();
    ZSTestSingleton(const ZSTestSingleton&)=delete;
    ZSTestSingleton& operator =(const ZSTestSingleton&)= delete;
    void uninitialize();

private slots:
    void onTest();

private:
    ZSTestSingleton();
    static int m_count;
};

#endif // ZSTESTSINGLETON_H
