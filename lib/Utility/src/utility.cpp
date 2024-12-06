#include "utility.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
Utility::Utility() {}

ComboxData::ComboxData(QString jsonString)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject obj = doc.object();
    index = obj.value("index").toInt();
    QVariant items = obj.value("items").toVariant();
    this->items = items.toStringList();
}

ComboxData::ComboxData(int index, QStringList items)
    : index(index), items(items)
{
}

QString ComboxData::toString()
{
    QJsonObject obj
    {
        {"index", index},
        {"items", QJsonArray::fromStringList(items)}
    };
    QJsonDocument doc(obj);
    return doc.toJson();
}
