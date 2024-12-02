#include "configDialog.h"
#include <QFormLayout>
ConfigDialog::ConfigDialog(QString name, QWidget *parent)
    : Widget(new QWidget(), parent)
{
    this->setWindowFlag(Qt::Dialog);
    this->getTitleBar()->getMinButton()->setVisible(false);
    this->getTitleBar()->getMaxButton()->setVisible(false);
    this->_config = new TConfig(name, this);
}

ConfigDialog::~ConfigDialog()
{
}

void ConfigDialog::initUi()
{
    QFormLayout *layout = new QFormLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    QMap<QString, QVariant> map = this->_config->readAll();
    for (auto it = map.begin(); it != map.end(); it++)
    {
        QLabel *label = new QLabel(it.key(), this);
        layout->addWidget(label);
    }
}

void ConfigDialog::initConnect()
{
}

void ConfigDialog::accept()
{
}

void ConfigDialog::reject()
{
}

void ConfigDialog::exec()
{
}

void ConfigDialog::saved(bool init)
{
}
