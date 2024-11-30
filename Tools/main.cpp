#include <QApplication>
#include <LTextEdit.h>
#include "mainwindow.h"
#include "center.h"
#include "QAbstractPlugin.h"
// #include "LSqlExecutor.h"
#include "config.h"
#include "LCore"
using namespace ljz;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Config *config = new Config("config.ini");
    LJsonConfig configs("config.ini");
    Center *center = new Center();
    MainWindow w(center);
    w.show();

    return a.exec();
}