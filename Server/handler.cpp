#include "handler.h"
#include <QThread>'
#include <QtNetwork>
Handler::Handler()
{

}

Handler::Handler(Server*server, QJsonObject &obj,ResoureClass *res){
    this->server=server;
    this->obj=obj;
    this->res=res;
//    qDebug()<<"data:"<<obj["data"].toString()<<"\n";
    qDebug()<<"What:"<<obj["what"].toInt()<<"\n";
    switch (obj["what"].toInt()) {
    case Handler::SIGNIN:
        this->dealSignIn();
        break;
    case Handler::LOGIN:
        this->dealLogin();
        break;
    case Handler::FINDPASSD:
        this->dealFindPassd();
        break;
    case Handler::SENDMSG:
        this->dealSendMsg();
        break;
    case Handler::SENDFILE:
        break;

    case Handler::ADDFRIEND:
        this->addFriend();
        break;
    case Handler::LINKFRIEND:
        this->dealLinkFriend();
        break;
    case LOGOUT:
        this->dealLogOut();
        break;
    default:
        break;
    }
}

void Handler::dealSignIn(){
    if(server->userList.contains(obj["user"].toString())){
        QJsonObject replyObj;
        replyObj["what"]=Handler::SIGNIN;
        replyObj["err"]=Handler::EXITUSER;
        Handler::sendMsg(res->socket,replyObj);
        return;
    }
    QString user=obj["user"].toString();
    QString passd=obj["passd"].toString();
    QString key=obj["key"].toString();
    SaveUser *saveuser=new SaveUser;
    saveuser->user=user;
    saveuser->passd=passd;
    saveuser->key=key;
    server->userList[user]=saveuser;

    server->friendMap[user]=QJsonArray();
    QJsonObject replyObj;
    replyObj["what"]=Handler::SIGNIN;
    replyObj["err"]=Handler::NOERR;
    sendMsg(res->socket,replyObj);
    QThread::msleep(150);

    QJsonObject newObj;
    newObj["what"]=ADDFRIEND;
    newObj["err"]=NOERR;
    newObj["user"]=user;

    for (int i = 0; i < server->socketMap.size(); ++i) {
        if(server->socketMap[i]->userName=="")
            continue;
        sendMsg(server->socketMap[i]->socket,newObj);
    }

//    sendMsg(res->socket,newObj);
}

void Handler::dealLogin(){
    QJsonObject replyObj;
    QString user=obj["user"].toString();
    replyObj["what"]=Handler::LOGIN;
    if(!server->userList.contains(user)){
        replyObj["err"]=Handler::NONUSER;
        Handler::sendMsg(res->socket,replyObj);
        return;
    }

    if(server->userList[user]->passd!=obj["passd"].toString())     {
        replyObj["err"]=Handler::PASSDERR;
        Handler::sendMsg(res->socket,replyObj);
        return;
    }

    for (int i = 0; i < server->socketMap.size(); ++i) {
        if(server->socketMap[i]->userName==user){
            replyObj["err"]=Handler::UNKNOWN;
            sendMsg(res->socket,replyObj);
            return;
        }
    }

    int port=server->getcreatePort();
    int udpport=server->getcreatePort();

    if(port==-1||udpport==-1){
        replyObj["err"]=Handler::UNKNOWN;
        sendMsg(res->socket,replyObj);
        return;
    }

    replyObj["user"]=user;
    replyObj["err"]=Handler::NOERR;
    replyObj["tcp"]=port;
    replyObj["udp"]=udpport;

    QJsonArray friends;

    QMap<QString,SaveUser*>::iterator it=server->userList.begin();
    for(;it!=server->userList.end();++it){
        if(it.value()->user==user)
            continue;
        QJsonObject fo;
        fo["user"]=it.value()->user;
        friends.append(fo);
    }

    QJsonArray offMsgArr;
    transMsg(offMsgArr);
    replyObj["friends"]=friends;
    replyObj["offMsg"]=offMsgArr;
    replyObj["what"]=LOGIN;
    Handler::sendMsg(res->socket,replyObj);

    notifyFriend(obj["user"].toString(),res->socket->peerAddress().toString(),udpport,port);

    server->setUserName(obj["user"].toString(),res->rowPosition,udpport,port);
    res->userName=obj["user"].toString();
    res->port=port;
    res->udpport=udpport;


}

void Handler::dealLogOut()
{
    QString user=obj["user"].toString();
    for (int i = 0; i < server->socketMap.size(); ++i) {
        if(server->socketMap[i]->userName==user){
            server->socketMap[i]->userName="";
            server->setUserName("",server->socketMap[i]->rowPosition,"","");
            break;
        }
    }
}

void Handler::notifyFriend(const QString &Name,const QString& ip,int udp,int tcp)
{
    QJsonObject notifyObj;
    notifyObj["what"]=LINKFRIEND;
    notifyObj["udp"]=udp;
    notifyObj["tcp"]=tcp;
    notifyObj["friend"]=Name;
    notifyObj["ip"]=ip;
    notifyObj["err"]=NOERR;
    for (int i = 0; i < server->socketMap.size(); ++i) {
        if(server->socketMap[i]->userName=="")
            continue;
        sendMsg(server->socketMap[i]->socket,notifyObj);
    }
}

void Handler::dealFindPassd(){
    QJsonObject replyObj;
    replyObj["what"]=Handler::FINDPASSD;
    if(!server->userList.contains(obj["user"].toString())){
        replyObj["err"]=Handler::NONUSER;
        Handler::sendMsg(res->socket,replyObj);
        return;
    }
    QString result;

    if(server->userList[obj["user"].toString()]->key!=obj["key"].toString()){
        replyObj["err"]=Handler::KEYERR;
        Handler::sendMsg(res->socket,replyObj);
        return;
    }

    replyObj["err"]=Handler::NOERR;
    replyObj["passd"]=server->userList[obj["user"].toString()]->passd;
    Handler::sendMsg(res->socket,replyObj);

}

void Handler::dealSendMsg(){
    TransTable table;

    table.data=obj["data"].toString();
    table.user=obj["d_account"].toString();
    table.contact=obj["user"].toString();
    this->server->transMap.push_back(table);
    qDebug()<<table.contact<<"转发消息给"<<table.user<<":"<<table.data;
}

void Handler::dealSendFile(){

}

void Handler::dealLinkFriend()
{

    QJsonObject replyObj;
    replyObj["what"]=Handler::LINKFRIEND;
    QString _friend=obj["friend"].toString();
    replyObj["friend"]=_friend;
    qDebug()<<"连接朋友:"<<"原:"<<res->userName<<"目的："<<_friend;
    for (int i = 0; i < server->socketMap.size(); ++i) {
        if(server->socketMap[i]->userName==_friend){
        QString address=server->socketMap[i]->socket->peerAddress().toString()=="127.0.0.1"?Handler::getIp():server->socketMap[i]->socket->peerAddress().toString();
            replyObj["err"]=Handler::NOERR;
            replyObj["ip"]=server->socketMap[i]->socket->peerAddress().toString();
            replyObj["tcp"]=server->socketMap[i]->port;
            replyObj["udp"]=server->socketMap[i]->udpport;
            Handler::sendMsg(res->socket,replyObj);
            return;
        }
    }
    qDebug()<<"连接朋友失败";
    replyObj["err"]=Handler::NOONLINE;
    Handler::sendMsg(res->socket,replyObj);
}

void Handler::addFriend(){
    QJsonObject replyObj;
    replyObj["what"]=Handler::ADDFRIEND;
    if(!server->userList.contains(obj["newfriend"].toString())){
        replyObj["err"]=Handler::NONUSER;
        Handler::sendMsg(res->socket,replyObj);
        return;
    }
    qDebug()<<"addfriend:"<<obj["user"].toString();;
    QJsonArray &userFriends=server->friendMap[obj["user"].toString()];
    userFriends.append(obj["newfriend"].toString());
    QString newFriend=obj["newfriend"].toString();
    QJsonArray &friends=server->friendMap[newFriend];
    friends.append(obj["user"].toString());

    replyObj["newfriend"]=newFriend;
    replyObj["err"]=Handler::NOERR;
    Handler::sendMsg(res->socket,replyObj);

    for (int i = 0; i < server->socketMap.size(); ++i) {
        if(server->socketMap[i]->userName==newFriend){
            QJsonObject replyObj2;
            replyObj2["what"]=Handler::ADDFRIEND;
            replyObj2["newfriend"]=obj["user"].toString();
            replyObj2["err"]=Handler::NOERR;
            Handler::sendMsg(server->socketMap[i]->socket,replyObj2);
            break;
        }
    }

}

QString Handler::getIp()
{
    QList<QHostAddress> adlist=QNetworkInterface::allAddresses();
    foreach(QHostAddress address, adlist){
        if(address.protocol() == QAbstractSocket::IPv4Protocol&&address!=QHostAddress::Null&&address!=QHostAddress::LocalHost)
           return address.toString();
    }
    return "127.0.0.1";
}

void Handler::transMsg(QJsonArray &arr)
{
    QString user=obj["user"].toString();
    QVector<TransTable>::iterator it;
    QVector<TransTable> &list=server->transMap;
    for(int i=0;i<list.count();){
        if(user==list[i].user){
            QJsonObject friendObj;
            friendObj["friend"]=list[i].contact;
            friendObj["msg"]=list[i].data;
            arr.append(friendObj);
            qDebug()<<user<<"收到来自"<<list[i].contact<<"的离线消息:"+list[i].data;
            list.remove(i);
        }
        else i++;
    }

}

void Handler::sendMsg(QTcpSocket *socket, QJsonObject &sendObj){
    QJsonDocument doc;
    doc.setObject(sendObj);
    QByteArray arr=doc.toJson();
    QDataStream os(&arr,QIODevice::ReadWrite);
    socket->write(arr);
    socket->waitForBytesWritten();
    socket->flush();
}
