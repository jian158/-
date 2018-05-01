#include "itemtable.h"
#include "fileutil.h"
FriendObj::FriendObj()
{

}

FriendObj::FriendObj(const QString &IP,const QString &_friend)
{
    this->Ip=IP;
    this->_friend=_friend;
}

FriendObj::FriendObj(const QString &IP, const QString &_friend, int row)
{
    this->Ip=IP;
    this->_friend=_friend;
    this->row=row;
}

ReceiveFileClass::ReceiveFileClass()
{
}

void ReceiveFileClass::updateSpeed()
{
    if(timer.isTimeOut){
        dataSpeed=dataSpeed>=2000?dataSpeed>>1:dataSpeed;
    }
    else if(dataSpeed<50000){
        dataSpeed=dataSpeed>=32000?dataSpeed+1000:dataSpeed<<1;
    }
    else if(dataSpeed>50000){
        dataSpeed=50000;
    }
}


ReceiveFileClass::~ReceiveFileClass()
{
    if(timer._timer.isActive()){
        timer.stop();
    }
}

Message::Message(int type, QString data)
{
    this->type=type;
    this->data=data;
}

