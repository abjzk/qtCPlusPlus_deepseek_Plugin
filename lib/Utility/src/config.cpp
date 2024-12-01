#include "config.h"
#include <QApplication>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

Config::Config(const QString& name,QObject *parent)
:_name(name),QObject(parent)
{
    excuteSql = new LSqlExecutor(QApplication::applicationDirPath() + "/config.db");
    checkTable();
}
Config::~Config()
{
    delete excuteSql;
}

bool Config::read(const QString &key, QVariant &value)
{
    return false;
}

bool Config::read(QMap<QString, QVariant> &map)
{
    return false;
}

bool Config::write(const QString &key, QVariant &value)
{
    return false;
}

bool Config::write(QMap<QString, QVariant> &map)
{
    return false;
}
void Config::checkTable()
{
    QString sql= R"(-- 检查表是否存在，如果不存在则创建表
CREATE TABLE IF NOT EXISTS Config (
    key TEXT PRIMARY KEY,     -- 字段 key
    value TEXT,               -- 字段 value
    name TEXT                 -- 字段 name
);)";
    excuteSql->executeNonQuery(sql);
}
