#include "QAbstractPlugin.h"

QAbstractPlugin::QAbstractPlugin(TConfig *config,QWidget *parent)
    : QWidget(parent)
{
    this->_config = new TConfig(this->name(),this);
}