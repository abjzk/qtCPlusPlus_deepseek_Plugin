#ifndef DeepSeekPlugin_GLOBAL_H
#define DeepSeekPlugin_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DeepSeekanalysisPlugin_LIBRARY)
#define DeepSeekanalysisPlugin_EXPORT Q_DECL_EXPORT
#else
#define DeepSeekanalysisPlugin_EXPORT Q_DECL_IMPORT
#endif

#endif // DeepSeekPlugin_GLOBAL_H
