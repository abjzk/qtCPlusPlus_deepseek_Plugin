#include "mainwindow.h"
#include "qhotkey.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QHotkey *key = new QHotkey(this);
    key->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_1), true);
    connect(key, &QHotkey::activated, [=]()
    {
        QMessageBox::information(this, "Hotkey Press", "Control + 1 pressed!");
    });
}

MainWindow::~MainWindow()
{
}
