#include "zwjsbridgeobject.h"
#include "zwlog.h"

ZWJSBridgeObject::ZWJSBridgeObject(QObject *parent)
    :QObject(parent)
{

}

ZWJSBridgeObject::~ZWJSBridgeObject()
{

}

void ZWJSBridgeObject::doTest(const QString &text, int type)
{
    ZW_LOG_INFO(QString("text=%1,type = 2").arg(text).arg(type));
}

void ZWJSBridgeObject::test()
{
    ZW_LOG_INFO("ssssss");
}
