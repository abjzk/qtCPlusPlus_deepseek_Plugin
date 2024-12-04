#pragma once
#include "LSqlExecutor.h"
#include <Utility_global.h>

// 写入配置时的事件
struct WriteConfigEvent
{
    //key
    QString key;
    // 旧值
    QVariant oldValue;
    // 新值，在Before时修改该值，会根据修改后的值写入
    QVariant newValue;
    // 值的类型
    QString type;
    // 是否有效，该值决定了是否写入
    bool isValid = true;
    // 信息，该值为错误信息
    QString message;
};

// 定义两个回调函数，分别是写入配置文件前和写入配置文件后的回调函数
typedef std::function<void(WriteConfigEvent &event)> WriteConfigBeforeCallback;
typedef std::function<void(WriteConfigEvent &event)> WriteConfigAfterCallback;

using namespace ljz;
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
        Color = 10
    };
    TConfig(const QString &name, QObject *parent = nullptr);
    ~TConfig();
    bool read(const QString &key, QVariant &value);
    QVariant read(const QString &key);
    bool read(QMap<QString, QVariant> &map);
    QMap<QString, QVariant> readAll();
    bool write(QString key, QVariant &value,QString &message);
    bool write(QMap<QString, QVariant> &map);
    void registerConfig(const QString &key, const QString &description, const Type &type, const QVariant &defaultValue, bool isShow = true);
    QList<QVariantMap> readAllAndDescription();
    void registerWriteConfigBeforeCallback(WriteConfigBeforeCallback callback) { writeConfigBeforeCallback = callback; }
    void registerWriteConfigAfterCallback(WriteConfigAfterCallback callback) { writeConfigAfterCallback = callback; }

private:
    void checkTable();
    // 回调函数
    WriteConfigBeforeCallback writeConfigBeforeCallback = nullptr;
    WriteConfigAfterCallback writeConfigAfterCallback = nullptr;

public:
    static Type getTypeFromString(const QString &type);
    static QString getTypeToString(const Type &type);
    static QString valueToString(const QVariant &value, const QString &type);
    static QVariant stringToValue(const QString &value, const QString &type);
};