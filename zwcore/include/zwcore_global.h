#ifndef ZWCORE_GLOBAL_H
#define ZWCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ZWCORE_LIBRARY)
#  define ZWCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ZWCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ZWCORE_GLOBAL_H
