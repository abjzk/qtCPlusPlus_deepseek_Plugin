#include "QAbstractPlugin.h"

QAbstractPlugin::QAbstractPlugin(QObject *parent)
    : QObject(parent)
{
}

QWidget *QAbstractPlugin::start(TConfig *config)
{
    _config = config;
    return nullptr;
}

void QAbstractPlugin::stop()
{
    delete _config;
    _config = nullptr;
}
