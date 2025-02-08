#include "config.h"
#include <QtTest>
#include "DeepSeek.h"
class TestConfig : public QObject
{
    Q_OBJECT
private slots:
    void registerConfig();
    void writeConfig();
    void deepSeek();
};