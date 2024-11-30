#include "qcoreapplication.h"
#include "config.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Config *config = new Config("config.ini");
    return 0;
}