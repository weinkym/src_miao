#include "zpeerobjectdata.h"
#include <QImage>

ZPeerObjectData::ZPeerObjectData()
{
    m_isRepeat = false;
}

void ZPeerObjectData::requestAvatar()
{
    if(m_isRepeat)
    {
        return;
    }
    ZRequestAction *action = ZPublicAction::createAvatarAction(m_url);
    connect(action,SIGNAL(sigFinished(ZRequestAction::ZRequestResponse)),this,SLOT(onFinished(ZRequestAction::ZRequestResponse)));
    action->trigger();
}

void ZPeerObjectData::onFinished(const ZRequestAction::ZRequestResponse &response)
{
    LOG_FUNCTION;
    bool ok = m_image.loadFromData(response.data);
    LOG_TEST(QString("ok = %1").arg(ok?"true":"false"));
    LOG_TEST(QString("response.data.size = %1").arg(response.data.size()));
    LOG_TEST(QString("image.size = %1").arg(m_image.size().width()));
    emit sigUpdate();
}
