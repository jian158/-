#ifndef LOG_H
#define LOG_H
#include <QString>

class Log //调试类
{
public:
    Log();
    static void LogInfo(const QString &log);
    static void LogError(const QString &log);
};

#endif // LOG_H
