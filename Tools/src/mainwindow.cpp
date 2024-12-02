#include "mainwindow.h"
#include <QHBoxLayout>
#include <QTimer>

TitleBar::TitleBar(QWidget *parent)
    :LBaseTitleBar(parent)
{
}

QWidget *TitleBar::getMaxButton()
{
    return nullptr;
}
void TitleBar::updateIcon()
{

}
void TitleBar::closeButtonClick()
{
    
}

void TitleBar::minButtonClick()
{
}

MainWindow::MainWindow(QWidget *mainWidget, QWidget *parent)
    :Widget(mainWidget, parent)
{
    this->initUi();
    this->initConnect();
    this->systemSettingsChangedSlot();
}

MainWindow::~MainWindow()
{

}

void MainWindow::systemSettingsChangedSlot()
{
    Widget::systemSettingsChangedSlot();
    this->_setButton->setIcon(setIconColor(QIcon(":/res/icon/set.png"),oppositeColor(this->palette().window().color())));
}

void MainWindow::initUi()
{
    _setButton->setIcon(QIcon(":res/Tools.ico"));
    _setButton->setFixedSize(25, 25);
    this->getTitleBar()->addWidget(_setButton);
}

void MainWindow::initConnect()
{
}

