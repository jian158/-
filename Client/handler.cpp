#include "handler.h"
#include <QDebug>
#include <QMessageBox>
#include "timeutil.h"
#include "log.h"
#include <QThread>
Handler::Handler()
{

}

Handler::Handler(MainWindow *mw, QJsonObject &obj){
    this->mw=mw;
    this->obj=obj;
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
    case Handler::LINKFRIEND:
        this->dealLinkFriend();
        break;
    case Handler::RECEIVEMSG:
        break;
    case Handler::RECEIVEFILE:
        break;
    case Handler::GETFRIENDINFO:
        this->dealFriendInfo();
        break;
    default:
        break;
    }
}

void Handler::dealSignIn(){
    if(obj["err"].isNull()){
        QMessageBox::information(mw,"提示","注册异常",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    int errType=obj["err"].toInt();
    if(errType==Handler::EXITUSER){
        QMessageBox::information(mw,"提示","此用户已经存在",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if(errType==Handler::NOERR){
        QMessageBox::information(mw,"提示","注册成功",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

void Handler::dealLogin(){
    int errType=obj["err"].toInt();
    if(errType==Handler::NONUSER){
        QMessageBox::information(mw,"提示","没有此用户",QMessageBox::Yes);
    }
    else if(errType==Handler::PASSDERR){
         QMessageBox::information(mw,"提示","密码错误",QMessageBox::Yes);
    }
    else if(errType==Handler::UNKNOWN)
        QMessageBox::information(mw,"提示","登录异常请重新登录",QMessageBox::Yes);
    else if(errType==Handler::NOERR){
        mw->initServer(obj["tcp"].toInt(),obj["udp"].toInt());
        mw->loginSucceed(obj["user"].toString());
        mw->initFriendsList(obj["friends"].toArray());

        QJsonArray arr=obj["offMsg"].toArray();
        for (int i = 0; i < arr.size(); ++i) {
            QJsonObject msgObj=arr[i].toObject();
            QString _friend=msgObj["friend"].toString();
            mw->friendList[_friend]->msgSum++;
            mw->friendList[_friend]->text.push_back(Message(1,msgObj["msg"].toString()));
        }

        QMap<QString,FriendObj*>::iterator it;
        for(it=mw->friendList.begin();it!=mw->friendList.end();++it){
            if(it.value()->msgSum>0){
                mw->listview->setMsg(it.value()->row,it.value()->msgSum);
            }
        }

        QMessageBox::information(mw,"提示","登录成功",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

void Handler::dealFindPassd(){
    int errType=obj["err"].toInt();
    if(errType==Handler::NONUSER){
        QMessageBox::information(mw,"提示","没有此用户",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if(errType==Handler::KEYERR){
         QMessageBox::information(mw,"提示","key错误",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if(errType==Handler::NOERR){
        QMessageBox::information(mw,"提示","密码为:"+obj["passd"].toString(),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

void Handler::dealSendMsg(){
    QString _friend=obj["user"].toString();
    FriendObj *item=mw->friendList[_friend];

    Message msg(1,obj["data"].toString());
    item->text.push_back(msg);
    if(_friend==mw->currentfreind){
        item->msgSum=0;
        mw->appenMessage(msg);
    }
    else{
        item->msgSum++;
        mw->listview->setMsg(item->row,item->msgSum);
    }
    qDebug()<<"Receive:"<<obj["data"].toString();
}



void Handler::dealFriendInfo()
{
    QString _friend=obj["user"].toString();
    QJsonObject replyobj;
    replyobj["user"]=mw->state.getUserName();
    replyobj["udp"]=QString::number(mw->udpSocket->localPort());
    sendMsg(mw->friendList[_friend]->socket,replyobj);
}


void Handler::dealLinkFriend()
{
    QString _friend=obj["friend"].toString();
    FriendObj *item=mw->friendList[_friend];
    item->linking=false;
    int errType=obj["err"].toInt();
    if(errType==Handler::NOONLINE){
        Log::LogInfo(mw->state.getUserName()+"连接"+_friend+"失败\n");
        return;
    }

    item->socket=new QTcpSocket();
    item->socket->connectToHost(obj["ip"].toString(),obj["tcp"].toInt());
    mw->addconnect(item->socket);
    item->isLinked=true;
    item->udpport=obj["udp"].toInt();
    Log::LogInfo(mw->state.getUserName()+"连接"+_friend+"成功\n");
}

void Handler::sendMsg(QTcpSocket *socket, QJsonObject &sendObj){
    QJsonDocument doc;            //声明解析json对象
    doc.setObject(sendObj);
    QByteArray arr=doc.toJson();  //将Json对象转化成字节数组
    QDataStream os(&arr,QIODevice::ReadWrite);
    socket->write(arr);          //用TCP套接字发送到服务器
}

