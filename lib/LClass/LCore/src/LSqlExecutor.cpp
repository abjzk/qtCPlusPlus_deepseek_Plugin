#include "LSqlExecutor.h"

jzk::LSqlExecutor::LSqlExecutor(const QString &sqlLitepath, QObject *parent, QString name)
    :QObject(parent)
{
    if (name == "")
        name = sqlLitepath;
    if (QSqlDatabase::contains(name)) {
        db = QSqlDatabase::database(name);
    }
    else {
        db = QSqlDatabase::addDatabase("QSQLITE", name);
        db.setDatabaseName(sqlLitepath);
    }
}