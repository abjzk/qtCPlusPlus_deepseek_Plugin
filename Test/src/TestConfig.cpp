#include "TestConfig.h"

void TestConfig::registerConfig()
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
    QCOMPARE(config->read("DateTime").toDateTime(), QDateTime::fromString("2020-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss"));
    QCOMPARE(config->read("Color").value<QColor>(), QColor(255, 0, 0));
    QCOMPARE(config->read("File").toString(), "D:\\test.txt");
    QCOMPARE(config->read("String").toString(), "test");
    QCOMPARE(config->read("Int").toInt(), 1);
    QCOMPARE(config->read("Bool").toBool(), true);
    QCOMPARE(config->read("Float").toFloat(), 1.1f);
    QCOMPARE(config->read("Double").toDouble(), 1.1);
    QCOMPARE(config->read("Directory").toString(), "D:\\test");
    QCOMPARE(config->read("Date").toDate(), QDate::fromString("2020-01-01", "yyyy-MM-dd"));
    QCOMPARE(config->read("Time").toTime(), QTime::fromString("00:00:00", "hh:mm:ss"));
}

void TestConfig::writeConfig()
{
    TConfig *config = new TConfig("config.ini");

    // 1. 注册初始配置，确保有数据可测试
    config->registerConfig("DateTime", "名称", TConfig::Type::DateTime, QDateTime::fromString("2020-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss"));
    config->registerConfig("Int", "名称", TConfig::Type::Int, 1);
    QString message;
    // 2. 测试 key 存在且类型匹配
    QVariant newDateTime = QDateTime::fromString("2024-12-03 12:00:00", "yyyy-MM-dd hh:mm:ss");
    bool writeResult1 = config->write("DateTime", newDateTime,message);
    QCOMPARE(writeResult1, true); // 更新应该成功
    QCOMPARE(config->read("DateTime").toDateTime(), newDateTime); // 验证值已更新

    // 3. 测试 key 存在但类型不匹配
    QVariant invalidValue = QString("Invalid DateTime");
    bool writeResult2 = config->write("DateTime", invalidValue,message);
    QCOMPARE(writeResult2, false); // 更新失败，类型不合法
    QCOMPARE(config->read("DateTime").toDateTime(), newDateTime); // 值应保持不变

    // 4. 测试 key 不存在
    QVariant newValue = 42;
    bool writeResult3 = config->write("NonExistentKey", newValue,message);
    QCOMPARE(writeResult3, false); // 更新失败，因为 key 不存在

    // 5. 测试 key 存在并更新其他类型
    QVariant newInt = 99;
    bool writeResult4 = config->write("Int", newInt,message);
    QCOMPARE(writeResult4, true); // 更新成功
    QCOMPARE(config->read("Int").toInt(), newInt); // 验证值已更新

    // 6. 测试边界情况：空字符串值
    QVariant emptyString = QString("");
    config->registerConfig("String", "名称", TConfig::Type::String, "test");
    bool writeResult5 = config->write("String", emptyString,message);
    QCOMPARE(writeResult5, true); // 应该允许空字符串更新
    QCOMPARE(config->read("String").toString(), emptyString.toString()); // 验证值更新为空字符串

    // 7. 测试边界情况：null 值
    QVariant nullValue = QVariant();
    bool writeResult6 = config->write("String", nullValue,message);
    QCOMPARE(writeResult6, false); // 应该失败，因为 null 无法匹配类型

    delete config;
}