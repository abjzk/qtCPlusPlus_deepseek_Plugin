#include "QAbstractPlugin.h"

QAbstractPlugin::QAbstractPlugin(TConfig *config,QObject *parent)
    : QObject(parent), _config(config)
{
    _config->setParent(this);
     _config->registerWriteConfigBeforeCallback([this](WriteConfigEvent &event)->void { 
        this->writeConfigBeforeEvent(event);
    });
    _config->registerWriteConfigAfterCallback([this](WriteConfigEvent &event)->void { 
        this->writeConfigAfterEvent(event);
    });
    this->registerConfig();
}

QWidget *QAbstractPlugin::start()
{
    return nullptr;
}

void QAbstractPlugin::stop()
{
    delete _config;
    _config = nullptr;
}

void QAbstractPlugin::registerConfig()
{
}

void QAbstractPlugin::writeConfigBeforeEvent(WriteConfigEvent &event)
{
}

void QAbstractPlugin::writeConfigAfterEvent(WriteConfigEvent &event)
{
}
