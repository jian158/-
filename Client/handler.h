#ifndef HANDLER_H
#define HANDLER_H
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include "mainwindow.h"

class Handler      //消息处理类
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
        RECEIVEFILE,
        GETFRIENDINFO
    };
    enum FileWhat{
        newFile=10,
        receiveFile,
        endReceiveFile,
        RejectFile,
        endSendFile,
        DENY,
        OK
    };
    MainWindow *mw;
    QJsonObject obj;
public:
    Handler();
    Handler(MainWindow *mw,QJsonObject &obj);
    void dealSignIn();
    void dealLogin();
    void dealFindPassd();
    void dealSendMsg();
    void dealLinkFriend();
    void transMsg();
    void dealOffMsg();
    void dealFriendInfo();
    static void sendMsg(QTcpSocket *socket,QJsonObject &sendObj);
};

#endif // HANDLER_H
