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
    QApplication::setFont(QFont("Microsoft YaHei UI"));
    Center *center = new Center();
    MainWindow w(center);
    w.show();
    return a.exec();
}