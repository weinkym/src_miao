#include "cscreenshottimer.h"

CScreenShotTimer::CScreenShotTimer(const QString &prefix)
    :m_prefix(prefix)
    ,m_time(QTime::currentTime())
{

}

CScreenShotTimer::~CScreenShotTimer()
{
    QTime currentTime = QTime::currentTime();
    int msecs = m_time.msecsTo(currentTime);
    if(msecs < 0)
    {
        msecs = currentTime.msecsTo(m_time);
    }
    C_SCREENSHOTSHARED_LOG(m_prefix + QString(" %1ms").arg(msecs))
}
