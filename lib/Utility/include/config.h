#pragma once
#include "LSqlExecutor.h"
#include <Utility_global.h>
#include "utility.h"
/**
 * @brief 对于不同的参数格式进行处理，以及写入配置前后的处理
 * @param
 * (QString key, QVariant value, QString type, QString description, bool isShow = true)
 * 或者()
 * @details ConfigItem
 */

class UTILITY_EXPORT ConfigItem
{
public:
    ConfigItem() {}
    ConfigItem(QString key, QVariant value, QString type, QString description, bool isShow = true) : key(key), value(value), type(type), description(description), isShow(isShow) {}
    ~ConfigItem() {}
    QString key;
    QVariant value;
    QString type;
    QString description;
    bool isShow = true;
    QVariantMap data() const { return QVariantMap{{"key", key}, {"value", value}, {"type", type}, {"description", description}, {"isShow", isShow}}; }
    QString valueString() const;
};
// 写入配置时的事件
struct WriteConfigEvent
{
    // key
    QString key;
    // 旧值
    ConfigItem oldItem;
    // 新值，在Before时修改该值，会根据修改后的值写入
    ConfigItem newItem;
    // 值的类型
    QString type;
    // 是否有效，该值决定了是否写入
    bool isValid = true;
    // 信息，该值为错误信息
    QString message;
    QVariant newValue() const { return newItem.value; }
    QVariant oldValue() const { return oldItem.value; }
    // 取消写入
    void cancel(QString message_)
    {
        isValid = false;
        message = message_;
    }
};
struct ReadConfigEvent
{
    ConfigItem *item;
    QVariant value() const { return item->value; }
};

// 定义两个回调函数，分别是写入配置文件前和写入配置文件后的回调函数
typedef std::function<void(WriteConfigEvent &event)> WriteConfigBeforeCallback;
typedef std::function<void(WriteConfigEvent &event)> WriteConfigAfterCallback;
// 定义一个回调函数，分别是读取配置文件前和读取配置文件后的回调函数
typedef std::function<void(ReadConfigEvent &event)> ReadConfigBeforeCallback;
typedef std::function<void(ReadConfigEvent &event)> ReadConfigAfterCallback;

using namespace jzk;
class UTILITY_EXPORT TConfig : public QObject
{
    Q_OBJECT
private:
    QString _name;
    LSqlExecutor *excuteSql;

public:
    enum Type
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
        Color = 10,
        Combox = 11
    };
    TConfig(const QString &name, QObject *parent = nullptr);
    ~TConfig();
    bool read(ConfigItem &item);
    bool readAll(QList<ConfigItem> &items);
    ConfigItem read(QString key);
    bool write(QString key, QVariant &value, QString &message);
    bool write(QMap<QString, QVariant> &map);
    void registerConfig(const QString &key, const QString &description, const Type &type, const QVariant &defaultValue, bool isShow = true);
    void registerWriteConfigBeforeCallback(WriteConfigBeforeCallback callback) { writeConfigBeforeCallback = callback; }
    void registerWriteConfigAfterCallback(WriteConfigAfterCallback callback) { writeConfigAfterCallback = callback; }
    void registerReadConfigBeforeCallback(ReadConfigBeforeCallback callback) { readConfigBeforeCallback = callback; }
    void registerReadConfigAfterCallback(ReadConfigAfterCallback callback) { readConfigAfterCallback = callback; }

private:
    void checkTable();
    // 回调函数
    WriteConfigBeforeCallback writeConfigBeforeCallback = nullptr;
    WriteConfigAfterCallback writeConfigAfterCallback = nullptr;
    ReadConfigBeforeCallback readConfigBeforeCallback = nullptr;
    ReadConfigAfterCallback readConfigAfterCallback = nullptr;

public:
    static Type getTypeFromString(const QString &type);
    static QString getTypeToString(const Type &type);
    //对应Type的字符串

    static QString valueToString(const QVariant &value, const QString &type);
    //对应Type的具体值
    static QVariant stringToValue(const QString &value, const QString &type);
};
