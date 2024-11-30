#include <QApplication>
#include <LTextEdit.h>
#include "mainwindow.h"
#include "center.h"
#include "../include/QAbstractPlugin.h"
#include "../include/config.h"

using namespace ljz;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Config *config = new Config("test");

    Center *center = new Center();
    MainWindow w(center);
    w.show();

    return a.exec();
}