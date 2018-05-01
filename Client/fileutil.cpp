#include "fileutil.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <QDebug>
#define FileDir QString("data\\")
FileUtil::FileUtil()
{

}

void FileUtil::readMessage(const QString &userName,QMap<QString, FriendObj *> map)
{
    createPath();
    QFile file(FileDir+userName+"chat.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"打开chat.txt错误";
        return;
    }
    QByteArray bytes=file.readAll();
    QJsonDocument doc=QJsonDocument::fromJson(bytes);
    QJsonObject parseObj=doc.object();
    QJsonArray arr=parseObj["chat"].toArray();
    for (int i = 0; i < arr.size(); ++i) {
        QJsonObject obj=arr[i].toObject();
        QJsonArray msgarr=obj["chat"].toArray();
        QString _friend=obj["friend"].toString();
        map[_friend]->msgSum=obj["msgSum"].toInt();
        for (int i = 0; i < msgarr.size(); ++i) {
            QJsonObject msgObj=msgarr[i].toObject();
            map[_friend]->text.push_back(Message(msgObj["type"].toInt(),msgObj["data"].toString()));
        }
    }
    file.close();
}

void FileUtil::saveMsg(const QString &userName, QMap<QString, FriendObj *> map)
{
    createPath();
    QFile file(FileDir+userName+"chat.txt");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"打开chat.txt错误";
        return;
    }
    QJsonObject saveObj;
    QJsonArray arr;
    QMap<QString,FriendObj*>::iterator it;
    for(it=map.begin();it!=map.end();++it){
        QJsonObject obj;
        QJsonArray msgarr;
        for(int i=0;i<it.value()->text.size();++i){
            QJsonObject msgObj;
            msgObj["type"]=it.value()->text[i].type;
            msgObj["data"]=it.value()->text[i].data;
            msgarr.append(msgObj);
        }
        obj["msgSum"]=it.value()->msgSum;
        obj["chat"]=msgarr;
        obj["friend"]=it.value()->_friend;
        arr.append(obj);
    }
    saveObj["chat"]=arr;
    QJsonDocument doc(saveObj);
    file.write(doc.toJson());
    file.close();
}

void FileUtil::ReadConf(QString &fileDir, int &WinSize)
{
    createPath();
    QFile file(FileDir+"conf.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"打开conf.txt错误";
        return;
    }
    QByteArray bytes=file.readAll();
    QJsonDocument doc=QJsonDocument::fromJson(bytes);
    QJsonObject obj=doc.object();
    fileDir=obj["filedir"].toString();
    WinSize=obj["winsize"].toInt();
    file.close();
}

void FileUtil::SaveConf(const QString &fileDir, const int &WinSize)
{
    createPath();
    QFile file(FileDir+"conf.txt");
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    QJsonObject saveObj;
    saveObj["filedir"]=fileDir;
    saveObj["winsize"]=WinSize;
    QJsonDocument doc(saveObj);
    file.write(doc.toJson());
    file.close();
}

void FileUtil::createPath()
{
    if(FileDir.length()>0){
        QDir dir(FileDir);
        if(!dir.exists()){
            dir.mkpath(FileDir);
        }
    }
}


