#ifndef CSCREENSHOTTIMER_H
#define CSCREENSHOTTIMER_H
#include <QString>
#include <QTime>
#include "cscreenshot_global.h"

class CScreenShotTimer
{

public:
    CScreenShotTimer(const QString &prefix);
    virtual ~CScreenShotTimer();

private:
    QString m_prefix;
    QTime m_time;
};

#endif // CSCREENSHOTTIMER_H
