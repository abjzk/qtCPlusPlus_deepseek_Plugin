#include "config.h"
#include <QApplication>


QString ConfigItem::valueString() const
{
    return TConfig::valueToString(value, type);
}

TConfig::TConfig(const QString &name, QObject *parent)
    : _name(name), QObject(parent)
{
    excuteSql = new LSqlExecutor(QApplication::applicationDirPath() + "/config.db");
    checkTable();
}
TConfig::~TConfig()
{
    delete excuteSql;
}



bool TConfig::read(ConfigItem &item)
{
    ReadConfigEvent event = {&item};
    QString key = item.key;
    if(this->readConfigBeforeCallback != nullptr)
        this->readConfigBeforeCallback(event);
    QString sql = QString("SELECT key,value,type,description,isShow FROM Config WHERE key = '%1' and name = '%2'").arg(item.key).arg(_name);
    auto row = excuteSql->executeFirstRow(sql);
    if (row.size() <= 0) return false;
    item.key = key;
    item.value = TConfig::stringToValue(row["value"].toString(), row["type"].toString());
    item.type = row["type"].toString();
    item.description = row["description"].toString();
    item.isShow = row["isShow"].toBool();
    if(this->readConfigAfterCallback != nullptr)
        this->readConfigAfterCallback(event);
    item.key = key;
    return true;
}

bool TConfig::readAll(QList<ConfigItem> &items)
{
    QString sql = QString("SELECT key FROM Config WHERE name = '%1'").arg(_name);
    QVariantList keys = excuteSql->executeFirstColumn(sql);
    for (auto key : keys)
    {
        ConfigItem item;
        item.key = key.toString();
        if(read(item))
            items.append(item);
    }
    return true;
}

ConfigItem TConfig::read(QString key)
{
    ConfigItem item;
    item.key = key;
    read(item);
    return item;
}

bool TConfig::write(QString key, QVariant &value,QString& message)
{
    // 查询key是否存在
    QString checkSql = QString("SELECT COUNT(*) FROM Config WHERE key = '%1' AND name = '%2'").arg(key).arg(_name);
    int count = excuteSql->executeScalar(checkSql, 0);
    if (count == 0){
        message = QString("key:%1不存在").arg(key);
        return false;
    }
    else
    {
        // key存在，进行类型验证和更新操作
        QString typeSql = QString("SELECT key,value,type,description,isShow FROM Config WHERE key = '%1' AND name = '%2'").arg(key).arg(_name);
        QVariantMap row = excuteSql->executeFirstRow(typeSql);
        QString type = row["type"].toString();
        QString valueStr = row["value"].toString();
        ConfigItem oldValue;
        oldValue.key = row["key"].toString();
        oldValue.description = row["description"].toString();
        oldValue.isShow = row["isShow"].toBool();
        oldValue.value = TConfig::stringToValue(valueStr, type);
        oldValue.type = type;
        ConfigItem newValue;
        newValue.key = row["key"].toString();
        newValue.description = row["description"].toString();
        newValue.isShow = row["isShow"].toBool();
        newValue.value = value;
        newValue.type = type;
        WriteConfigEvent event = {key, oldValue, newValue, type, true, ""};
        if (this->writeConfigBeforeCallback != nullptr)
            this->writeConfigBeforeCallback(event);
        if (!event.isValid)
        {
            message = event.message;
            return false;
        }
        QString updateSql = QString("UPDATE Config SET value = '%1' WHERE key = '%2' AND name = '%3'")
                            .arg(valueToString(event.newItem.value, event.type))
                            .arg(event.newItem.key)
                            .arg(_name);
        excuteSql->executeNonQuery(updateSql);
        if (this->writeConfigAfterCallback != nullptr)
            this->writeConfigAfterCallback(event);
        return true;
    }
}



bool TConfig::write(QMap<QString, QVariant> &map)
{
    for (auto it = map.begin(); it != map.end(); it++)
    {
        QString message;
        write(it.key(), it.value(), message);
    }
    return true;
}
void TConfig::registerConfig(const QString &key, const QString &description, const Type &type, const QVariant &defaultValue, bool isShow)
{
    // 检查是否存在这个key,不存在就插入
    int count = excuteSql->executeScalar(QString("SELECT COUNT(*) FROM Config WHERE key = '%1' and name = '%2'").arg(key).arg(_name), 0);
    if (count > 0)
    {
        return;
    }
    else
    {
        QString sql = QString("INSERT INTO Config (key, description, type, value, name, isShow) VALUES ('%1', '%2', '%3', '%4', '%5', %6)")
                          .arg(key)
                          .arg(description)
                          .arg(getTypeToString(type))
                          .arg(valueToString(defaultValue, getTypeToString(type)))
                          .arg(_name)
                          .arg(isShow);
        excuteSql->executeNonQuery(sql);
    }
}
void TConfig::checkTable()
{
    QString sql = R"(-- 检查表是否存在，如果不存在则创建表
CREATE TABLE IF NOT EXISTS Config (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    key TEXT,     -- 字段 key
    value TEXT,               -- 字段 value
    name TEXT,                 -- 字段 name
    description TEXT,         -- 字段 description
    type TEXT,                -- 字段 type
    isShow INTEGER             -- 字段 isShow
);)";
    excuteSql->executeNonQuery(sql);
};

TConfig::Type TConfig::getTypeFromString(const QString &type)
{
    if (type == "int")
        return Type::Int;
    else if (type == "string")
        return Type::String;
    else if (type == "bool")
        return Type::Bool;
    else if (type == "float")
        return Type::Float;
    else if (type == "double")
        return Type::Double;
    else if (type == "datetime")
        return Type::DateTime;
    else if (type == "date")
        return Type::Date;
    else if (type == "time")
        return Type::Time;
    else if (type == "file")
        return Type::File;
    else if (type == "directory")
        return Type::Directory;
    else if (type == "color")
        return Type::Color;
    else if (type == "combox")
        return Type::Combox;
    else
        return Type::String;
}
QString TConfig::getTypeToString(const TConfig::Type &type)
{
    switch (type)
    {
    case Type::Int:
        return "int";
    case Type::String:
        return "string";
    case Type::Bool:
        return "bool";
    case Type::Float:
        return "float";
    case Type::Double:
        return "double";
    case Type::DateTime:
        return "datetime";
    case Type::Date:
        return "date";
    case Type::Time:
        return "time";
    case Type::File:
        return "file";
    case Type::Directory:
        return "directory";
    case Type::Color:
        return "color";
    case Type::Combox:
        return "combox";
    default:
        return "string";
    }
}
QString TConfig::valueToString(const QVariant &value, const QString &type)
{
    TConfig::Type configType = TConfig::getTypeFromString(type);
    switch (configType)
    {
    case Type::Int:
        return QString::number(value.toInt());
    case Type::String:
        return value.toString();
    case Type::Bool:
        return QString::number(value.toBool());
    case Type::Float:
        return QString::number(value.toFloat());
    case Type::Double:
        return QString::number(value.toDouble());
    case Type::DateTime:
        return value.toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    case Type::Date:
        return value.toDate().toString("yyyy-MM-dd");
    case Type::Time:
        return value.toTime().toString("hh:mm:ss");
    case Type::File:
        return value.toString();
    case Type::Directory:
        return value.toString();
    case Type::Color:
        return value.value<QColor>().name();
    case Type::Combox:
        return value.value<ComboxData>().toString();
    default:
        return value.toString();
    }
}
QVariant TConfig::stringToValue(const QString &value, const QString &type)
{
    TConfig::Type configType = TConfig::getTypeFromString(type);
    switch (configType)
    {
    case Type::Int:
        return value.toInt();
    case Type::String:
        return value;
    case Type::Bool:
        return value.toUpper() == "1";
    case Type::Float:
        return value.toFloat();
    case Type::Double:
        return value.toDouble();
    case Type::DateTime:
        return QDateTime::fromString(value, "yyyy-MM-dd hh:mm:ss");
    case Type::Date:
        return QDate::fromString(value, "yyyy-MM-dd");
    case Type::Time:
        return QTime::fromString(value, "hh:mm:ss");
    case Type::File:
        return value;
    case Type::Directory:
        return value;
    case Type::Color:
        return QColor(value);
    case Type::Combox:
        return QVariant::fromValue(ComboxData(value));
    default:
        return value;
    }
}


