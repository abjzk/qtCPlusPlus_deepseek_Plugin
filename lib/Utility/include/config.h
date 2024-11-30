#include "LCore"
#include <Utility_global.h>

class UTILITY_EXPORT Config : public QObject
{
    Q_OBJECT
private:
    QString _name;
    LSqlExecutor *excuteSql;

public:
    Config(const QString &name);
    ~Config();
    bool read(const QString &key, QVariant &value);
    bool read(QMap<QString, QVariant> &map);
    bool write(const QString &key, QVariant &value);
    bool write(QMap<QString, QVariant> &map);

private:
    void checkTable();
};