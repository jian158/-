#ifndef RESOURECLASS_H
#define RESOURECLASS_H
#include<QTcpSocket>

class ResoureClass
{
public:
    QTcpSocket *socket;
    QString userName="";//用户名
    int rowPosition;    //在界面表中序号
    int port;           //tcp端口
    int udpport;       //udp端口
public:
    ResoureClass();
    ResoureClass(QTcpSocket *socket,int rowPosition);
};

#endif // RESOURECLASS_H
