#include "cscreenshottimer.h"

CScreenShotTimer::CScreenShotTimer(const QString &prefix)
    :m_prefix(prefix)
    ,m_time(QTime::currentTime())
{
    C_SCREENSHOT_LOG_INFO(QString("Enter function:%1").arg(prefix));
}

CScreenShotTimer::~CScreenShotTimer()
{
    QTime currentTime = QTime::currentTime();
    int msecs = m_time.msecsTo(currentTime);
    if(msecs < 0)
    {
        msecs = currentTime.msecsTo(m_time);
    }
    C_SCREENSHOT_LOG_INFO(QString("Leave function(%1ms) %2").arg(msecs).arg(m_prefix));
}
