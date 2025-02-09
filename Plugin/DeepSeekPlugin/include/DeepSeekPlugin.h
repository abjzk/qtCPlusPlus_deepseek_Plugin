#ifndef DeepSeekPlugin_H
#define DeepSeekPlugin_H

#include "DeepSeekPlugin_global.h"
#include "AbstractPlugin.h"
#include "config.h"
#include "DeepSeekPluginWidget.h"

class DeepSeekPlugin_EXPORT DeepSeekPlugin : public AbstractPlugin
{
    Q_OBJECT
public:
    DeepSeekPlugin(Logger *logger, TConfig *config, QObject *parent = nullptr);
    ~DeepSeekPlugin() override;
    virtual QString group() override;
    virtual QString name() override;
    virtual QString version() override;
    virtual QString author() override;
    virtual QString description() override;
    virtual QIcon icon() override;
    virtual QWidget *start() override;
    virtual void stop() override;
    virtual void writeConfigBeforeEvent(WriteConfigEvent &event) override;
    virtual void writeConfigAfterEvent(WriteConfigEvent &event) override;
    virtual void readConfigBeforeEvent(ReadConfigEvent &event) override;
    virtual void readConfigAfterEvent(ReadConfigEvent &event) override;
    virtual void registerConfig() override;

private:
    DeepSeekWidget *_widget = nullptr;
};

class DeepSeekPlugin_EXPORT DeepSeekPluginFactory : public QObject, public PluginFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID AbstractPlugin_IID)
    Q_INTERFACES(PluginFactory)
public:
    AbstractPlugin *create(Logger *logger, TConfig *config) override { return new DeepSeekPlugin(logger, config); };
    ~DeepSeekPluginFactory() override = default;
};

#endif // DeepSeekPlugin_H
