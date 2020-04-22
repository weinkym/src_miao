#ifndef CLOGSETTING_H
#define CLOGSETTING_H
#include <QDebug>
#include <QTime>
#include <QThread>
#include "clogcontent.h"
#include "clogdefines.h"
#include "clogengine.h"
#include "clogfunchelper.h"

using namespace C_LOG;

#define C_LOG_FIELD_FILE (QString("[FILE=%1]").arg(QString(__FILE__)))
#define C_LOG_FIELD_FUN (QString("[FUN=%1]").arg(QString(Q_FUNC_INFO)))
#define C_LOG_FIELD_LINE (QString("[LINE=%1]").arg(QString("0000%1").arg(__LINE__).right(4)))

#define C_LOG_OUTPUT(type,level,log) CLogEngine::getInstance()->outLog(type,level,C_LOG_FIELD_LINE,(QString(log)),C_LOG_FIELD_FUN,C_LOG_FIELD_FILE)


#define C_LOG_P(V1) (QString(#V1"=%1").arg(createLogContent(V1)))
#define C_LOG_P2(V1,V2) (C_LOG_P(V1)+","+C_LOG_P(V2))
#define C_LOG_P3(V1,V2,V3) (C_LOG_P2(V1,V2)+","+C_LOG_P(V3))
#define C_LOG_P4(V1,V2,V3,V4) (C_LOG_P2(V1,V2)+","+C_LOG_P2(V3,V4))
#define C_LOG_P5(V1,V2,V3,V4,V5) (C_LOG_P3(V1,V2,V3)+","+C_LOG_P2(V4,V5))
#define C_LOG_P6(V1,V2,V3,V4,V5,V6) (C_LOG_P3(V1,V2,V3)+","+C_LOG_P3(V4,V5,V6))

#define C_LOG_FUNCTION CLogFuncHelper logFuncHelper(C_LOG_FIELD_LINE,C_LOG_FIELD_FUN,C_LOG_FIELD_FILE,C_LOG::INFO)
#define C_LOG_FUNCTION_DEBUG CLogFuncHelper logFuncHelper(C_LOG_FIELD_LINE,C_LOG_FIELD_FUN,C_LOG_FIELD_FILE,C_LOG::DEBUG)


#define C_LOG_NORMAL_LEVEL(level,log) C_LOG_OUTPUT(C_LOG::TYPE_NORMAL,level,log)

#define C_LOG_TEST_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG::DEBUG,log)

#define C_LOG_DEBUG_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG::DEBUG,log)
#define C_LOG_INFO_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG::INFO,log)
#define C_LOG_WARNING_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG::WARNING,log)
#define C_LOG_ERROR_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG::ERROR,log)
#define C_LOG_FATAL_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG::FATAL,log)

#define C_LOG_TEST(log) C_LOG_TEST_TYPE(C_LOG::TYPE_NORMAL,log)
#define C_LOG_DEBUG(log) C_LOG_DEBUG_TYPE(C_LOG::TYPE_NORMAL,log)
#define C_LOG_INFO(log) C_LOG_INFO_TYPE(C_LOG::TYPE_NORMAL,log)
#define C_LOG_WARNING(log) C_LOG_WARNING_TYPE(C_LOG::TYPE_NORMAL,log)
#define C_LOG_ERROR(log) C_LOG_ERROR_TYPE(C_LOG::TYPE_NORMAL,log)
#define C_LOG_FATAL(log) C_LOG_FATAL_TYPE(C_LOG::TYPE_NORMAL,log)

#define C_LOG_OUT_V(V1) C_LOG_NORMAL_LEVEL(C_LOG::INFO,C_LOG_P(V1))
#define C_LOG_OUT_V2(V1,V2) C_LOG_NORMAL_LEVEL(C_LOG::INFO,C_LOG_P2(V1,V2))
#define C_LOG_OUT_V3(V1,V2,V3) C_LOG_NORMAL_LEVEL(C_LOG::INFO,C_LOG_P3(V1,V2,V3))
#define C_LOG_OUT_V4(V1,V2,V3,V4) C_LOG_NORMAL_LEVEL(C_LOG::INFO,C_LOG_P4(V1,V2,V3,V4))
#define C_LOG_OUT_V5(V1,V2,V3,V4,V5) C_LOG_NORMAL_LEVEL(C_LOG::INFO,C_LOG_P5(V1,V2,V3,V4,V5))
#define C_LOG_OUT_V6(V1,V2,V3,V4,V5,V6) C_LOG_NORMAL_LEVEL(C_LOG::INFO,C_LOG_P6(V1,V2,V3,V4,V5,V6))

#define C_LOG_V(level,prefix,V1) C_LOG_NORMAL_LEVEL(level,C_LOG_P2(prefix,V1))
#define C_LOG_V2(level,prefix,V1,V2) C_LOG_NORMAL_LEVEL(level,C_LOG_P3(prefix,V1,V2))
#define C_LOG_V3(level,prefix,V1,V2,V3) C_LOG_NORMAL_LEVEL(level,C_LOG_P4(prefix,V1,V2,V3))

#endif // CLOGSETTING_H
