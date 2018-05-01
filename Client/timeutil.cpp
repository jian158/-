#include "timeutil.h"
#include <QDateTime>
#include "log.h"
#include "fileutil.h"
TimeUtil::TimeUtil()
{

}

QString TimeUtil::getTime()
{

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    return current_date;
}

quint64 TimeUtil::current()
{
    QDateTime current_time = QDateTime::currentDateTime();
    return current_time.toMSecsSinceEpoch();
}


Timer::Timer(){
    QString file;
    int win;
    FileUtil::ReadConf(file,win);
    for (int i = 0; i < win; ++i){
        receiveCache[i]=0;
    }
    connect(&_timer,SIGNAL(timeout()),this,SLOT(reSend())); //设置超时信号函数
}


void Timer::start(int ms)
{
    _start=TimeUtil::current();
    _timer.start(ms);
}

void Timer::start()
{
    long long rtt=_end-_start;
    if(rtt<30)
        rtt=30;
    _start=TimeUtil::current();
     _timer.start(rtt+30);
}

void Timer::stop()
{
    _end=TimeUtil::current();
    isTimeOut=false;
    _timer.stop();
}

void Timer::freeCache()
{
    for(int i=0;i<sum;++i){
        delete cache[i].data;
    }
}

void Timer::saveFile(QFile *file,long long &currentSize)
{
    for(int i=0;i<sum;i++){
        file->write(*receiveCache[i]);
        currentSize+=receiveCache[i]->size();
        delete receiveCache[i];
        receiveCache[i]=0;
    }
}


void Timer::setData(QByteArray *bytes)
{
    delete this->bytes;
    this->bytes=bytes;
}

void Timer::setSocket(QUdpSocket *socket, QHostAddress *sender, quint16 sendport)
{
    this->udpSocket=socket;
    this->sender=sender;
    this->sendport=sendport;
}


void Timer::reSend()
{
    isTimeOut=true;
    for(int i=0;i<sum;++i){
        udpSocket->writeDatagram(*cache[i].data,*sender,sendport);
    }
}
