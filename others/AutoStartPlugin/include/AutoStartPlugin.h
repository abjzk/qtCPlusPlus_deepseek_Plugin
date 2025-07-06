#ifndef AutoStartPlugin_H
#define AutoStartPlugin_H

#include "AutoStartPlugin_global.h"
#include "AbstractPlugin.h"
#include "config.h"


class AutoStartPlugin_EXPORT AutoStartPlugin : public AbstractPlugin
{
    Q_OBJECT
public:
    AutoStartPlugin(Logger * logger,TConfig *config,QObject *parent = nullptr);
    ~AutoStartPlugin() override;
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


class AutoStartPlugin_EXPORT AutoStartPluginFactory : public QObject, public PluginFactory
{
	Q_OBJECT
		Q_PLUGIN_METADATA(IID AbstractPlugin_IID)
		Q_INTERFACES(PluginFactory)
public:
	AbstractPlugin* create(Logger *logger,TConfig *config) override { return new AutoStartPlugin(logger,config); };
	~AutoStartPluginFactory() override = default;
} ;


#endif // AutoStartPlugin_H
