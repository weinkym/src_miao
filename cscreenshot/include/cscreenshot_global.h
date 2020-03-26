#ifndef _CSCREENSHOT_GLOBAL_H_
#define _CSCREENSHOT_GLOBAL_H_

#include <QtCore/qglobal.h>
#include <QDebug>
#
#include "cscreenshottimer.h"

#if defined(CSCREENSHOT_LIBRARY)
#  define CSCREENSHOTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CSCREENSHOTSHARED_EXPORT Q_DECL_IMPORT
#endif

#if 0
#include <QThread>
#define C_SCREENSHOT_LOG_HEAD (QString(" [") + QString(__FILE__) + QString(" : Line-") + QString::number(__LINE__) + QString("]"))
#define C_SCREENSHOT_LOG_THREAD (QString("[%1] ").arg((long)QThread::currentThreadId()))

#define C_SCREENSHOT_LOG_DEBUG(log) qDebug()<<(C_SCREENSHOT_LOG_THREAD + QString(log) + C_SCREENSHOT_LOG_HEAD)
#define C_SCREENSHOT_LOG_INFO(log) qInfo()<<(C_SCREENSHOT_LOG_THREAD + QString(log) + C_SCREENSHOT_LOG_HEAD)
#define C_SCREENSHOT_LOG_WARNING(log) qWarning()<<(C_SCREENSHOT_LOG_THREAD + QString(log) + C_SCREENSHOT_LOG_HEAD)
#define C_SCREENSHOT_LOG_CRITICAL(log) qCritical()<<(C_SCREENSHOT_LOG_THREAD + QString(log) + C_SCREENSHOT_LOG_HEAD)
#define C_SCREENSHOT_LOG_FATAL(log) qFatal()<<(C_SCREENSHOT_LOG_THREAD + QString(log) + C_SCREENSHOT_LOG_HEAD)

#define C_SCREENSHOT_LOG_FUNCTION CScreenShotTimer temp_screenShotTimer(Q_FUNC_INFO)

#define C_SCREENSHOT_LOG_TEST C_SCREENSHOT_LOG_DEBUG(QString("TTTTTTTTTTTTTV"))

#else
#define C_SCREENSHOT_LOG_HEAD
#define C_SCREENSHOT_LOG_THREAD
#define C_SCREENSHOT_LOG_TEST(log)
#define C_SCREENSHOT_LOG_DEBUG(log)
#define C_SCREENSHOT_LOG_INFO(log)
#define C_SCREENSHOT_LOG_WARNING(log)
#define C_SCREENSHOT_LOG_ERROR(log)

#define C_SCREENSHOT_LOG_FUNCTION

#define C_SCREENSHOT_LOG_TEST
#endif

#endif // _CSCREENSHOT_GLOBAL_H_
