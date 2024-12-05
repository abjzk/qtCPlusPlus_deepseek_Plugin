#ifndef ReplaceTemplatePlugin_H
#define ReplaceTemplatePlugin_H

#include "ReplaceTemplatePlugin_global.h"
#include "QAbstractPlugin.h"
#include "config.h"


class ReplaceTemplatePlugin_EXPORT ReplaceTemplatePlugin : public QAbstractPlugin
{
    Q_OBJECT
public:
    ReplaceTemplatePlugin(TConfig *config,QObject *parent = nullptr);
    ~ReplaceTemplatePlugin() override;
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
};


class ReplaceTemplatePlugin_EXPORT ReplaceTemplatePluginFactory : public QObject, public PluginFactory
{
	Q_OBJECT
		Q_PLUGIN_METADATA(IID QAbstractPlugin_IID)
		Q_INTERFACES(PluginFactory)
public:
	QAbstractPlugin* create(TConfig *config) override { return new ReplaceTemplatePlugin(config); };
	~ReplaceTemplatePluginFactory() override = default;
} ;


#endif // ReplaceTemplatePlugin_H

