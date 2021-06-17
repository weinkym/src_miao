#include "zstestsingleton.h"
#include "zwlog.h"

int ZSTestSingleton::m_count = 0;

void ZSTestSingleton::doTest()
{
    ZW_LOG_FUNCTION;
    auto value = (long long)this;
    ZW_LOG_INFO(ZW_LOG_P(value));
    ZW_LOG_INFO_V(value);
    QString temp = QString("%1").arg(10,0,2);
    ZW_LOG_INFO_V(temp);
    ZW_LOG_INFO_V(m_count);
    onTest();
}

void ZSTestSingleton::uninitialize()
{
    ZW_LOG_FUNCTION;
}

void ZSTestSingleton::onTest()
{
    ZW_LOG_FUNCTION;
}

ZSTestSingleton::ZSTestSingleton()
{
   ZW_LOG_FUNCTION;
   m_count++;
   ZW_LOG_INFO_V(m_count);
}
