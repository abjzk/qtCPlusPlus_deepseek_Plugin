#ifndef UTILITY_H
#define UTILITY_H

#include "Utility_global.h"
#include <QStringList>
#include <QMetaType>

struct UTILITY_EXPORT ComboxData
{
    ComboxData() {}
    ComboxData(QString jsonString);
    ComboxData(int index, QStringList items);
    int index = -1;
    QStringList items;
    QString toString();
};
Q_DECLARE_METATYPE(ComboxData)
class UTILITY_EXPORT Utility
{
public:
    Utility();
};

#endif // UTILITY_H
