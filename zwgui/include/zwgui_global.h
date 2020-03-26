#ifndef ZWGUI_GLOBAL_H
#define ZWGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ZWGUI_LIBRARY)
#  define ZWGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define ZWGUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ZWGUI_GLOBAL_H
