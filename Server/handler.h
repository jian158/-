#ifndef HANDLER_H
#define HANDLER_H
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "server.h"
#include <QTcpSocket>
#include "resoureclass.h"

#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include "user.h"

class Handler
{
public:
    enum Error{
        NOERR,
        EXITUSER,
        NONUSER,
        PASSDERR,
        KEYERR,
        NOONLINE,
        UNKNOWN
    };

    enum What{
        SIGNIN=0,
        LOGIN=1,
        FINDPASSD=2,
        SENDMSG=3,
        SENDFILE=4,
        ADDFRIEND,
        LINKFRIEND,
        RECEIVEMSG,
        LOGOUT,
    };
    Server* server;
    QJsonObject obj;
    ResoureClass *res;
public:
    Handler();
    Handler(Server* server,QJsonObject &obj,ResoureClass *res);
    void dealSignIn();
    void dealLogin();
    void dealLogOut();
    void notifyFriend(const QString &Name,const QString& ip,int udp,int tcp);
    void dealFindPassd();
    void dealSendMsg();
    void dealSendFile();
    void dealLinkFriend();
    static QString getIp();
    void transMsg(QJsonArray &arr);
    void addFriend();
    static void sendMsg(QTcpSocket *socket,QJsonObject &sendObj);
};

#endif // HANDLER_H
