#ifndef Z3UTILS_GLOBAL_H
#define Z3UTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(Z3UTILS_LIBRARY)
#  define Z3UTILSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define Z3UTILSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // Z3UTILS_GLOBAL_H
