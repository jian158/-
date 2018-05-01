#ifndef ITEMTABLE_H
#define ITEMTABLE_H
#include <QString>
#include <QTcpSocket>
#include <QFile>
#include "progressdialog.h"
#include "timeutil.h"
#include <QList>
class Message{  //消息封装
public:
    Message(int type=0,QString data="");
    int type;
    QString data;
};
class FriendObj  //好友对象类
{
public:
    FriendObj();

    FriendObj(const QString &IP,const QString &_friend);
    FriendObj(const QString &IP,const QString &_friend,int row);
public:
    int row;
    int msgSum=0;
    bool isLinked=false;
    bool linking=false;
    QTcpSocket *socket=nullptr;
    QString Ip="";
    QString _friend="";
    QVector<Message> text;
    int udpport;
    QFile *file=nullptr;

};



class ReceiveFileClass{
public:
    ReceiveFileClass();
    void updateSpeed();
    ~ReceiveFileClass();
public:
    QFile *file;
    long long  FileSize=0;
    long long currentSize=0;
    ProgressDialog *progess;
    int packageId=0;
    Timer timer;
    int dataSpeed=1000;
};



#endif // ITEMTABLE_H
