#pragma once
#include "QObject"
#include <QKeySequence>
#include "utility.h"
#include "config.h"
#include "Utility_global.h"
#include <QWidget>
class UTILITY_EXPORT QAbstractPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractPlugin(QObject *parent = nullptr);
    virtual ~QAbstractPlugin() = default;
    virtual QString group() = 0;
    virtual QString name() = 0;
    virtual QString version() = 0;
    virtual QString author() = 0;
    virtual QString description() = 0;
    virtual QIcon icon() = 0;
    virtual QWidget * start(TConfig *config);
    virtual void stop();
protected:
    TConfig *_config;
};

// 插件工厂
class UTILITY_EXPORT PluginFactory
{
public:
    /**
     * @brief 创建插件
     * @return
     */
    virtual QAbstractPlugin *create() = 0;
    virtual ~PluginFactory() = default;
};

#define QAbstractPlugin_IID "org.Tools.QAbstractPlugin"

Q_DECLARE_INTERFACE(PluginFactory, QAbstractPlugin_IID)