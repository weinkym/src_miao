#include "zstestsingleton.h"
#include "zwlog.h"

void ZSTestSingleton::doTest()
{
    ZW_LOG_FUNCTION;
    auto value = (long long)this;
    ZW_LOG_INFO(ZW_LOG_P(value));
    ZW_LOG_INFO_V(value);
    QString temp = QString("%1").arg(10,0,2);
    ZW_LOG_INFO_V(temp);
}
