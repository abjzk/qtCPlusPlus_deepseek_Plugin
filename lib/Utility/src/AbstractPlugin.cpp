#include "AbstractPlugin.h"
#include "Logger.h"
AbstractPlugin::AbstractPlugin(Logger* logger,TConfig *config,QObject *parent)
    : QObject(parent), _config(config),_logger(logger)
{
    _config->setParent(this);
    _logger->setParent(this);
    _logger->flush_on(_config->read("LogLevel").value.value<ComboxData>().currentText());
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

QWidget *AbstractPlugin::start()
{
    return nullptr;
}

void AbstractPlugin::stop()
{
    delete _config;
    _config = nullptr;
}

void AbstractPlugin::registerConfig()
{
    ComBox_CONFIG_REGISTER("LogLevel", "日志等级", QVariant::fromValue(ComboxData(2,Logger::levels())), true);   
}

void AbstractPlugin::writeConfigBeforeEvent(WriteConfigEvent &event)
{
}

void AbstractPlugin::writeConfigAfterEvent(WriteConfigEvent &event)
{
    if (event.key.contains("LogLevel"))
    {
        _logger->flush_on(_config->read("LogLevel").value.value<ComboxData>().currentText());
    }
}

void AbstractPlugin::readConfigBeforeEvent(ReadConfigEvent &event)
{
}

void AbstractPlugin::readConfigAfterEvent(ReadConfigEvent &event)
{
}
