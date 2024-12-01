#include "TestConfig.h"

void TestConfig::test()
{
    TConfig *config = new TConfig("config.ini");
    config->registerConfig("DateTime", "名称", TConfig::Type::DateTime, QDateTime::fromString("2020-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss"));
    config->registerConfig("Color", "名称", TConfig::Type::Color, QColor(255, 0, 0));
    config->registerConfig("File", "名称", TConfig::Type::File, "D:\\test.txt");
    config->registerConfig("String", "名称", TConfig::Type::String, "test");
    config->registerConfig("Int", "名称", TConfig::Type::Int, 1);
    config->registerConfig("Bool", "名称", TConfig::Type::Bool, true);
    config->registerConfig("Float", "名称", TConfig::Type::Float, 1.1f);
    config->registerConfig("Double", "名称", TConfig::Type::Double, 1.1);
    config->registerConfig("Directory", "名称", TConfig::Type::Directory, "D:\\test");
    config->registerConfig("Date", "名称", TConfig::Type::Date, QDate::fromString("2020-01-01", "yyyy-MM-dd"));
    config->registerConfig("Time", "名称", TConfig::Type::Time, QTime::fromString("00:00:00", "hh:mm:ss"));

    // 判断读取后是否和预期一致
    QCOMPARE(config->read("DateTime").toString(), "2020-01-01 00:00:00");
    QCOMPARE(config->read("Color").value<QColor>().name(), "#ff0000");
    QCOMPARE(config->read("File").toString(), "D:\\test.txt");
    QCOMPARE(config->read("String").toString(), "test");
    QCOMPARE(config->read("Int").toInt(), 1);
    QCOMPARE(config->read("Bool").toBool(), true);
    QCOMPARE(config->read("Float").toFloat(), 1.1f);
    QCOMPARE(config->read("Double").toDouble(), 1.1);
    QCOMPARE(config->read("Directory").toString(), "D:\\test");
    QCOMPARE(config->read("Date").toDate().toString("yyyy-MM-dd"), "2020-01-01");
    QCOMPARE(config->read("Time").toTime().toString("hh:mm:ss"), "00:00:00");
}