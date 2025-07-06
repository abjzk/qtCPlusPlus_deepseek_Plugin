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
    //两种数据库，deepseekconfig专门放聊天内容
    //config专门放各种插件参数
    //多种namespace jzk 根据包含头文件不同而不同，常见有用于weiget，和config sql的
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
