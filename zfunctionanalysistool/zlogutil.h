#ifndef ZLOGUTIL_H
#define ZLOGUTIL_H
#include <QDebug>

class ZLogUtil
{
public:
    ZLogUtil();
    ~ZLogUtil();
};

#define Z_LOG_DEBUG(X) qDebug()<<(X)
#define Z_LOG_WARNING(X) qWarning()<<(X)
#define Z_LOG_CRITICAL(X) qCritical()<<(X)

#define Z_LOG_LINE Z_LOG_DEBUG(QString("line=%1").arg(__LINE__))

void setLogFileName(const QString& fileName);
void debugMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // ZLOGUTIL_H
