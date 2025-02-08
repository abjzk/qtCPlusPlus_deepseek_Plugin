#include <QApplication>
#include <LTextEdit.h>
#include "mainwindow.h"
#include "center.h"
#include "AbstractPlugin.h"
// #include "LSqlExecutor.h"
#include "config.h"
#include "LCore"
using namespace ljz;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setFont(QFont("Microsoft YaHei UI"));
    TConfig *config = new TConfig("SYSTEM");
    Center *center = new Center(config);
    MainWindow w(config,center);
    config->setParent(&w);
    w.show();
    return a.exec();
}