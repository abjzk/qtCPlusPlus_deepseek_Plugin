#ifndef DEEPSEEK_GLOBAL_H
#define DEEPSEEK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DEEPSEEK_LIBRARY)
#define DEEPSEEK_EXPORT Q_DECL_EXPORT
#else
#define DEEPSEEK_EXPORT Q_DECL_IMPORT
#endif

#endif // DEEPSEEK_GLOBAL_H
