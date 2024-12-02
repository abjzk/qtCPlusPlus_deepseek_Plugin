#ifndef ImageToIcoPlugin_H
#define ImageToIcoPlugin_H

#include "ImageToIcoPlugin_global.h"
#include "QAbstractPlugin.h"


class ImageToIcoPlugin_EXPORT ImageToIcoPlugin : public QAbstractPlugin
{
    Q_OBJECT
public:
    ImageToIcoPlugin(QWidget *parent = nullptr) : QAbstractPlugin(parent) {}
    ~ImageToIcoPlugin() override = default;
    virtual QString group() override;
    virtual QString name() override;
    virtual QString version() override;
    virtual QString author() override;
    virtual QString description() override;
    virtual QIcon icon() override;
};


class ImageToIcoPlugin_EXPORT ImageToIcoPluginFactory : public QObject, public PluginFactory
{
	Q_OBJECT
		Q_PLUGIN_METADATA(IID QAbstractPlugin_IID)
		Q_INTERFACES(PluginFactory)
public:
	QAbstractPlugin* create() override { return new ImageToIcoPlugin(); };
	~ImageToIcoPluginFactory() override = default;
} ;


#endif // ImageToIcoPlugin_H
