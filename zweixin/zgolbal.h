#ifndef ZGOLBAL_H
#define ZGOLBAL_H
#include <QDebug>

#define LOG_FUNCTION qDebug()<<Q_FUNC_INFO;
#define LOG_ERROR(x) qDebug()<<x;
#define LOG_WARNING(x) qDebug()<<x;
#define LOG_INFO(x) qDebug()<<x;
#define LOG_DEBUG(x) qDebug()<<x;
#define LOG_TEST(x) qDebug()<<x;
#endif // ZGOLBAL_H
