#include <QApplication>
#include <LTextEdit.h>
#include "mainwindow.h"
#include "center.h"
#include "../include/QAbstractPlugin.h"
using namespace ljz;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Center *center = new Center();
    MainWindow w(center);
    w.show();

    return a.exec();
}