#include "config.h"
#include <QtTest>

class TestConfig : public QObject
{
    Q_OBJECT
private slots:
    void registerConfig();
    void writeConfig();
};