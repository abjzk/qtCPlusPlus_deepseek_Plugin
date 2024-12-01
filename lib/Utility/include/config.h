#pragma once
#include "LSqlExecutor.h"
#include <Utility_global.h>


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
    bool write(const QString &key, QVariant &value);
    bool write(QMap<QString, QVariant> &map);
    void registerConfig(const QString &key, const QString &description, const Type &type, const QVariant &defaultValue, bool isShow = true);

private:
    void checkTable();

public:
    static Type getTypeFromString(const QString &type);
    static QString getTypeToString(const Type &type);
    static QString valueToString(const QVariant &value, const QString &type);
    static QVariant stringToValue(const QString &value, const QString &type);
};