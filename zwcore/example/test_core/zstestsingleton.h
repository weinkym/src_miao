#ifndef ZSTESTSINGLETON_H
#define ZSTESTSINGLETON_H
#include "zwsingleton.h"

class ZSTestSingleton : public ZWSingleton<ZSTestSingleton>
{
    friend class ZWSingleton<ZSTestSingleton>;
public:
    void doTest();
    ZSTestSingleton(const ZSTestSingleton&)=delete;
    ZSTestSingleton& operator =(const ZSTestSingleton&)= delete;
private:
    ZSTestSingleton()=default;
};

#endif // ZSTESTSINGLETON_H
