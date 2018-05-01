#include "log.h"
#include <QFile>
Log::Log()
{

}

void Log::LogInfo(const QString &log)
{
    QFile file("log.txt");
    if(!file.open(QIODevice::Append))
        return;
    file.write(log.toUtf8());
    file.close();
}

void Log::LogError(const QString &log)
{
    QFile file("errlog.txt");
    if(!file.open(QIODevice::Append))
        return;
    file.write(log.toUtf8());
    file.close();
}
