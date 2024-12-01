#include "LCore"
#include <Utility_global.h>
enum class ConfigType
{
    Int = 0,
    String = 1,
    Bool = 2,
    Float = 3,
    Double = 4,
    DateTime = 5,
    Date = 6,
    Time = 7,
    File = 8,
    Directory = 9,
    Color = 10
};
ConfigType getTypeFromString(const QString& type)
{
    if (type == "int")
        return ConfigType::Int;
    else if (type == "string")
        return ConfigType::String;
    else if (type == "bool")
        return ConfigType::Bool;
    else if (type == "float")
        return ConfigType::Float;
    else if (type == "double")
        return ConfigType::Double;
    else if (type == "datetime")
        return ConfigType::DateTime;
    else if (type == "date")
        return ConfigType::Date;
    else if (type == "time")
        return ConfigType::Time;
    else if (type == "file")
        return ConfigType::File;
    else if (type == "directory")
        return ConfigType::Directory;
    else if (type == "color")
        return ConfigType::Color;
    else
        return ConfigType::String;
}

QString getTypeToString(const ConfigType& type)
{
    switch (type)
    {
    case ConfigType::Int:
        return "int";
    case ConfigType::String:
        return "string";
    case ConfigType::Bool:
        return "bool";
    case ConfigType::Float:
        return "float";
    case ConfigType::Double:
        return "double";
    case ConfigType::DateTime:
        return "datetime";
    case ConfigType::Date:
        return "date";
    case ConfigType::Time:
        return "time";
    case ConfigType::File:
        return "file";
    case ConfigType::Directory:
        return "directory";
    case ConfigType::Color:
        return "color";
    default:
        return "string";
    }
};
QString valueToString(const QVariant& value,const QString& type)
{
    ConfigType configType = getTypeFromString(type);
    switch (configType)
    {
    case ConfigType::Int:
        return QString::number(value.toInt());
    case ConfigType::String:
        return value.toString();
    case ConfigType::Bool:
        return QString::number(value.toBool());
    case ConfigType::Float:
        return QString::number(value.toFloat());
    case ConfigType::Double:
        return QString::number(value.toDouble());
    case ConfigType::DateTime:
        return value.toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    case ConfigType::Date:
        return value.toDate().toString("yyyy-MM-dd");
    case ConfigType::Time:
        return value.toTime().toString("hh:mm:ss");
    case ConfigType::File:
        return value.toString();
    case ConfigType::Directory:
        return value.toString();
    case ConfigType::Color:
        return value.value<QColor>().name();
    default:
        return value.toString();
    }
}
QVariant stringToValue(const QString& value,const QString& type)
{
    ConfigType configType = getTypeFromString(type);
    switch (configType)
    {
    case ConfigType::Int:
        return value.toInt();
    case ConfigType::String:
        return value;
    case ConfigType::Bool:
        return value.toUpper() == "TRUE";
    case ConfigType::Float:
        return value.toFloat();
    case ConfigType::Double:
        return value.toDouble();
    case ConfigType::DateTime:
        return QDate::fromString(value, "yyyy-MM-dd hh:mm:ss");
    case ConfigType::Date:
        return QDate::fromString(value, "yyyy-MM-dd");
    case ConfigType::Time:
        return QTime::fromString(value, "hh:mm:ss");
    case ConfigType::File:
        return value;
    case ConfigType::Directory:
        return value;
    case ConfigType::Color:
        return QColor(value);
    default:
        return value;
    }
}
class UTILITY_EXPORT Config : public QObject
{
    Q_OBJECT
private:
    QString _name;
    LSqlExecutor *excuteSql;

public:
    Config(const QString &name, QObject *parent = nullptr);
    ~Config();
    bool read(const QString &key, QVariant &value);
    bool read(QMap<QString, QVariant> &map);
    bool write(const QString &key, QVariant &value);
    bool write(QMap<QString, QVariant> &map);
    void registerConfig(const QString &key,const QString& description,const ConfigType& type,const QVariant& defaultValue) 
    {
        
    };

private:
    void checkTable();
};