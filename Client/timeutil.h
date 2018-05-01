#ifndef TIMEUTIL_H
#define TIMEUTIL_H
#include <QString>
#include <QTimer>
#include <QUdpSocket>
#include <QByteArray>
#include <QMutex>
#include <QFile>
class TimeUtil        //时间处理类
{
public:
    TimeUtil();
    static QString getTime();
    static quint64 current();
};

typedef struct Package{
    QByteArray *data;
    int packageId;
}Package;

class Timer:public QObject  //定时器用于超时重传
{
    Q_OBJECT
public:
    Timer();
    void start(int ms);
    void start();
    void stop();
    void freeCache();
    void saveFile(QFile *file,long long& currentSize);
    void setData(QByteArray *bytes);
    void setSocket(QUdpSocket *socket,QHostAddress *sender,quint16 sendport);
public:
    QUdpSocket *udpSocket=nullptr;
    QByteArray *bytes=0;
    QHostAddress *sender;
    quint16 sendport;
    QTimer _timer;
    Package cache[20];
    int sum=0;
    QByteArray* receiveCache[20];
    bool isTimeOut=false;
    long long _start=0,_end=0;
public slots:
    void reSend();
};

#endif // TIMEUTIL_H
