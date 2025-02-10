#include <QApplication>
#include <LTextEdit.h>
#include "mainwindow.h"
#include "center.h"
#include "AbstractPlugin.h"
// #include "LSqlExecutor.h"
#include "config.h"
#include "LCore"
using namespace jzk;
int main(int argc, char *argv[])
{
    _putenv_s("QT_FONT_DPI", "96");
    QApplication a(argc, argv);
    QApplication::setFont(QFont("Microsoft YaHei UI"));
    TConfig *config = new TConfig("SYSTEM");
    Center *center = new Center(config);
    MainWindow w(config, center);
    config->setParent(&w);
    w.show();
    return a.exec();
}