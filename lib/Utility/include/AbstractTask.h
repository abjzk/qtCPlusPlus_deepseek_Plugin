#pragma once
#include <QRunnable>
#include "Utility_global.h"


class UTILITY_EXPORT AbstractTask : public QRunnable
{
public:
    AbstractTask() = default;
    virtual void run() = 0;
};