#include "QAbstractPlugin.h"

QAbstractPlugin::QAbstractPlugin(QWidget *parent)
    : QWidget(parent)
{
}

void QAbstractPlugin::start(TConfig *config)
{
    _config = config;
}

void QAbstractPlugin::stop()
{
    delete _config;
}
