#include "mainwindow.h"
#include <QHBoxLayout>
#include <QTimer>
#include <QApplication>
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

MainWindow::MainWindow(TConfig *config,QWidget *mainWidget, QWidget *parent)
    :Widget(mainWidget, parent), _config(config)
{
    this->resize(1280, 720);
    // 移动到屏幕中央
    this->move(QGuiApplication::primaryScreen()->geometry().center() - this->rect().center());
    this->initUi();
    this->initConnect();
    this->systemSettingsChangedSlot();
}

MainWindow::~MainWindow()
{
    QApplication::quit();
}

void MainWindow::systemSettingsChangedSlot()
{
    Widget::systemSettingsChangedSlot();
}

void MainWindow::initUi()
{

    _setButton->setFixedSize(35, 35);
    _setButton->setIcon(QIcon(":res/icon/set.png"));
    _setButton->setCursor(Qt::PointingHandCursor);
    _setButton->setToolTip("系统设置");
    this->getTitleBar()->addWidget(_setButton);





}

void MainWindow::initConnect()
{
    connect(_setButton, &QPushButton::clicked, this, &MainWindow::showConfigDialog);
}

void MainWindow::showConfigDialog()
{
    if (dialog) return;
    dialog = new ConfigDialog(_config);
    connect(dialog, &ConfigDialog::closed, this, &MainWindow::reSet);
    this->setEnabled(false);
    dialog->exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Widget::closeEvent(event);
    QApplication::quit();
}

void MainWindow::reSet()
{
    this->setEnabled(true);
    if (dialog)
    {
        dialog->deleteLater();
        dialog = nullptr;
    }
}
