#ifndef DeepSeekPlugin_GLOBAL_H
#define DeepSeekPlugin_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DeepSeekPlugin_LIBRARY)
#define DeepSeekPlugin_EXPORT Q_DECL_EXPORT
#else
#define DeepSeekPlugin_EXPORT Q_DECL_IMPORT
#endif

#endif // DeepSeekPlugin_GLOBAL_H
