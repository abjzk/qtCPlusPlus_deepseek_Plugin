#ifndef TemplatePlugin_H
#define TemplatePlugin_H

#include "TemplatePlugin_global.h"
#include "QAbstractPlugin.h"
#include "config.h"


class TemplatePlugin_EXPORT TemplatePlugin : public QAbstractPlugin
{
    Q_OBJECT
public:
    TemplatePlugin(Logger * logger,TConfig *config,QObject *parent = nullptr);
    ~TemplatePlugin() override;
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


class TemplatePlugin_EXPORT TemplatePluginFactory : public QObject, public PluginFactory
{
	Q_OBJECT
		Q_PLUGIN_METADATA(IID QAbstractPlugin_IID)
		Q_INTERFACES(PluginFactory)
public:
	QAbstractPlugin* create(Logger *logger,TConfig *config) override { return new TemplatePlugin(logger,config); };
	~TemplatePluginFactory() override = default;
} ;


#endif // TemplatePlugin_H
