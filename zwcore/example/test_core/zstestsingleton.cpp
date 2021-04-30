#include "zstestsingleton.h"
#include "zwlog.h"

void ZSTestSingleton::doTest()
{
    ZW_LOG_FUNCTION;
    auto value = (long long)this;
    ZW_LOG_INFO(ZW_LOG_P(value));
    ZW_LOG_INFO_V(value);
}
