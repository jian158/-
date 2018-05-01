#ifndef FILESR_H
#define FILESR_H
#include <QUdpSocket>
#include "itemtable.h"
#include <QDataStream>
#include <QMessageBox>
//用于收发文件
void sendFile(QUdpSocket *udpSocket,const QHostAddress& sender,
quint16 senderPort,int winSize,QDataStream& ds,const QMap<int,ReceiveFileClass*> & sendFileMap){
    ReceiveFileClass *item=sendFileMap[senderPort];
    if(item==nullptr)
        return;
    int packageId;
    ds>>packageId;
    item->updateSpeed();
    if(packageId!=item->packageId){//包id不是预期跳过
        return;
    }
    else{
        item->currentSize+=item->dataSpeed*winSize;
        item->progess->setProgress(item->currentSize*100/item->FileSize);
    }

    item->timer.stop();//暂停定时器
    item->timer.freeCache();//释放超时资源
    item->timer.sum=0;
    int i=0;

    while(!item->file->atEnd()&&i<winSize){
        QByteArray *bytes=new QByteArray;
        QDataStream sendDs(bytes,QIODevice::WriteOnly);
        QByteArray data=item->file->read(item->dataSpeed);
        //通过QDataStream文件流将包类型、id、内容封装
        sendDs<<Handler::RECEIVEFILE<<item->packageId+i<<data;
        item->timer.cache[item->timer.sum++]={bytes,item->packageId+i};
        udpSocket->writeDatagram(*bytes,sender,senderPort);
        i++;
    }
    if(item->file->atEnd()&&i<winSize){
        QByteArray *bytes=new QByteArray;
         QDataStream sendDs(bytes,QIODevice::WriteOnly);
        sendDs<<Handler::endReceiveFile;
        item->timer.cache[item->timer.sum++]={bytes,item->packageId+i};
        udpSocket->writeDatagram(*bytes,sender,senderPort);
    }
    item->packageId+=winSize;
    item->timer.start();
}


void newFile(QWidget *win,QUdpSocket *udpSocket,const QHostAddress& sender,
             quint16 senderPort,QDataStream& ds,QMap<int,ReceiveFileClass*> & receiFileMap){

    QByteArray bytes;
    QDataStream sendDs(&bytes,QIODevice::WriteOnly);
    if(receiFileMap.contains(senderPort)){
        sendDs<<Handler::SENDFILE<<0;
        udpSocket->writeDatagram(bytes,sender,senderPort);
        return;
    }

    ReceiveFileClass *_class=new ReceiveFileClass;

    QString fileName;
    ds>>fileName>>_class->FileSize;
    _class->file=new QFile(fileName);

    if(!_class->file->open(QIODevice::WriteOnly)){
        return;
    }

    receiFileMap[senderPort]=_class;
    _class->progess=new ProgressDialog(win);
    _class->progess->setText("正在接收"+fileName);
    _class->progess->show();

    sendDs<<Handler::SENDFILE<<0;
    udpSocket->writeDatagram(bytes,sender,senderPort);
}


void ReceiveFile(QUdpSocket *udpSocket,const QHostAddress& sender,
                 quint16 senderPort,QDataStream& ds,QMap<int,ReceiveFileClass*> & receiFileMap,int winSize){
    int packageId;
    ds>>packageId;
    ReceiveFileClass *rc=receiFileMap[senderPort];
    if(rc==nullptr)
        return;
    if(packageId<rc->packageId||packageId>=rc->packageId+winSize||rc->timer.receiveCache[packageId%winSize]!=0){
        QByteArray bytes;
        QDataStream sendDs(&bytes,QIODevice::WriteOnly);
        sendDs<<Handler::SENDFILE<<rc->packageId;
        udpSocket->writeDatagram(bytes,sender,senderPort);
        return;
    }


    QByteArray *arr=new QByteArray;
    ds>>*arr;
    rc->timer.receiveCache[packageId%winSize]=arr;
    rc->timer.sum++;
    if(rc->timer.sum==winSize){
        rc->timer.saveFile(rc->file,rc->currentSize);
        rc->progess->setProgress(rc->currentSize*100/rc->FileSize);
        rc->packageId+=winSize;
        QByteArray bytes;
        QDataStream sendDs(&bytes,QIODevice::WriteOnly);
        sendDs<<Handler::SENDFILE<<rc->packageId;
        udpSocket->writeDatagram(bytes,sender,senderPort);
        rc->timer.sum=0;
    }
}

void endSendFile(QWidget *win,QMap<int,ReceiveFileClass*> & sendFileMap,quint16 senderPort){
    ReceiveFileClass *item=sendFileMap[senderPort];
    if(item==nullptr)
        return;
    sendFileMap.remove(senderPort);
    item->timer.stop();
    item->progess->close();
    QtConcurrent::run([item]{
        item->file->close();
        delete item;
    });
    QMessageBox::information(win,"提示","发送完成！");
}

void endReceiveFile(QWidget *win,QUdpSocket *udpSocket,QMap<int,ReceiveFileClass*> & receiFileMap,const QHostAddress& sender,quint16 senderPort){
    QByteArray bytes;
     QDataStream sendDs(&bytes,QIODevice::WriteOnly);
    if(!receiFileMap.contains(senderPort)){
        sendDs<<Handler::endSendFile;
        udpSocket->writeDatagram(bytes,sender,senderPort);
        return;
    }
    ReceiveFileClass *rc=receiFileMap[senderPort];
    rc->progess->close();
    receiFileMap.remove(senderPort);
    QtConcurrent::run([rc]{
        rc->timer.saveFile(rc->file,rc->currentSize);
        rc->file->close();
        delete rc;
    });

    sendDs<<Handler::endSendFile;
    udpSocket->writeDatagram(bytes,sender,senderPort);
    QMessageBox::information(win,"提示","接收完毕！");
}

#endif // FILESR_H
