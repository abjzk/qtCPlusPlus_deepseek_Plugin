#include "QAbstractPlugin.h"
#include "Logger.h"
QAbstractPlugin::QAbstractPlugin(Logger* logger,TConfig *config,QObject *parent)
    : QObject(parent), _config(config),_logger(logger)
{
    _config->setParent(this);
    _logger->setParent(this);
     _config->registerWriteConfigBeforeCallback([this](WriteConfigEvent &event)->void { 
        this->writeConfigBeforeEvent(event);
    });
    _config->registerWriteConfigAfterCallback([this](WriteConfigEvent &event)->void { 
        this->writeConfigAfterEvent(event);
    });
    _config->registerReadConfigBeforeCallback([this](ReadConfigEvent &event)->void {
        this->readConfigBeforeEvent(event);
    });
    _config->registerReadConfigAfterCallback([this](ReadConfigEvent &event)->void {
        this->readConfigAfterEvent(event);
    });
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
    ComBox_CONFIG_REGISTER("LogLevel", "日志等级", QVariant::fromValue(ComboxData(2,Logger::levels())), true);   
}

void QAbstractPlugin::writeConfigBeforeEvent(WriteConfigEvent &event)
{
}

void QAbstractPlugin::writeConfigAfterEvent(WriteConfigEvent &event)
{
}

void QAbstractPlugin::readConfigBeforeEvent(ReadConfigEvent &event)
{
}

void QAbstractPlugin::readConfigAfterEvent(ReadConfigEvent &event)
{
}
