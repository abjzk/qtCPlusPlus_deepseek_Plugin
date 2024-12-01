#include "QAbstractPlugin.h"

QAbstractPlugin::QAbstractPlugin(Config *config,QWidget *parent)
    : QWidget(parent)
{
    this->_config = new Config(this->name(),this);
}