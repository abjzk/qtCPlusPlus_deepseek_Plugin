#ifndef UTILITY_H
#define UTILITY_H

#include "Utility_global.h"
enum class LogLevel
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Fatal = 5
};
class UTILITY_EXPORT Utility
{
public:
    Utility();
};

#endif // UTILITY_H
