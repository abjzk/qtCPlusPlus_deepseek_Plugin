#ifndef TemplatePlugin_GLOBAL_H
#define TemplatePlugin_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TemplatePlugin_LIBRARY)
#define TemplatePlugin_EXPORT Q_DECL_EXPORT
#else
#define TemplatePlugin_EXPORT Q_DECL_IMPORT
#endif

#endif // TemplatePlugin_GLOBAL_H
