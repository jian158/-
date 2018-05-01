#include "user.h"
#include "QDataStream"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "encryption/qaeswrap.h"
#include <QDir>
#define FileDir QString("data\\")
#define KEY "1234"
User::User()
{

}

void User::updataTransMap(QVector<TransTable>& transMap){
    createPath();
    QFile file(FileDir+"tranlist.txt");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"打开tranlist.txt错误";
        return;
    }
    QVector<TransTable>::iterator it;
    qDebug()<<"tranMap Size:"<<transMap.size()<<"\n";
    QJsonObject saveObj;
    QJsonArray saveArr;
    for(it=transMap.begin();it!=transMap.end();++it){
        QJsonObject obj;
        obj["user"]=it->user;
        obj["friend"]=it->contact;
        obj["data"]=it->data;
        saveArr.append(obj);
    }
    saveObj["trans"]=saveArr;
    QJsonDocument doc;
    doc.setObject(saveObj);
    QAesWrap aes(KEY,KEY,QAesWrap::AES_256);

   file.write(aes.encrypt(doc.toJson(),QAesWrap::AES_CTR));
    file.close();
}

void User::readMessage(QVector<TransTable> &list)
{
    createPath();
    QFile file(FileDir+"tranlist.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"打开tranlist.txt错误";
        return;
    }
    QAesWrap aes(KEY,KEY,QAesWrap::AES_256);
    QByteArray bytes=aes.decrypt(file.readAll(),QAesWrap::AES_CTR);
    QJsonParseError parseError;
    QJsonDocument doc(QJsonDocument::fromJson(bytes,&parseError));

    if(parseError.error==QJsonParseError::NoError){
        QJsonArray saveArr=doc.object()["trans"].toArray();
        for (int i = 0; i < saveArr.size(); ++i) {
            QJsonObject obj=saveArr[i].toObject();
            TransTable table={obj["user"].toString(),obj["friend"].toString(),obj["data"].toString()};
            list.push_back(table);
        }
    }
    file.close();
}

void User::updataFriendsMap(QMap<QString, QJsonArray> &map)
{
    createPath();
    QFile file(FileDir+"friends.txt");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"打开friends.txt错误";
        return;
    }

    QMap<QString, QJsonArray>::iterator it;
    QJsonObject saveObj;
    QJsonArray objArr;
    for(it=map.begin();it!=map.end();++it){
        QJsonObject obj;
        obj.insert(it.key(),it.value());
        objArr.append(obj);
    }
    saveObj["friends"]=objArr;
    QJsonDocument doc(saveObj);
    QAesWrap aes(KEY,KEY,QAesWrap::AES_256);
    file.write(aes.encrypt(doc.toJson(),QAesWrap::AES_CTR));
    file.close();
}

void User::createPath()
{
    if(FileDir.length()>0){
        QDir dir(FileDir);
        if(!dir.exists()){
            dir.mkpath(FileDir);
        }
    }
}

void User::readFriends(QMap<QString, QJsonArray> &map)
{
    createPath();
    QFile file(FileDir+"friends.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"打开tranlist.txt错误";
        return;
    }
    QAesWrap aes(KEY,KEY,QAesWrap::AES_256);
    QByteArray bytes=aes.decrypt(file.readAll(),QAesWrap::AES_CTR);
    QJsonDocument doc=QJsonDocument::fromJson(bytes);
    QJsonArray saveArr=doc.object()["friends"].toArray();

    for (int i = 0; i < saveArr.size(); ++i) {
        QJsonObject obj=saveArr[i].toObject();
        map[obj.begin().key()]=obj.begin().value().toArray();
    }
    file.close();
}

void User::readUserList(QMap<QString,SaveUser*> &list)
{
    createPath();
    QFile file(FileDir+"user.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"打开user.txt错误";
        return;
    }
    QAesWrap aes(KEY,KEY,QAesWrap::AES_256);
    QByteArray bytes=aes.decrypt(file.readAll(),QAesWrap::AES_CTR);
    QJsonParseError parseError;
    QJsonDocument doc(QJsonDocument::fromJson(bytes,&parseError));

    if(parseError.error==QJsonParseError::NoError){
    QJsonArray arr=doc.object()["users"].toArray();
    qDebug()<<"read users size:"<<arr.size();
    for (int i = 0; i<arr.size(); ++i) {
        QJsonObject obj=arr[i].toObject();
        SaveUser *user=new SaveUser;
        user->user=obj["user"].toString();
        user->passd=obj["passd"].toString();
        user->key=obj["key"].toString();
        list[user->user]=user;
        qDebug()<<user->user;
        }
    }
    file.close();
}

void User::SaveUserList(QMap<QString,SaveUser*> &list)
{
    createPath();
    QFile file(FileDir+"user.txt");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"打开user.txt错误";
        return;
    }

    QJsonObject saveObj;
    QJsonArray saveArr;
    QMap<QString,SaveUser*>::iterator it;
    for(it=list.begin();it!=list.end();++it){
        QJsonObject obj;
        obj["user"]=it.value()->user;
        obj["passd"]=it.value()->passd;
        obj["key"]=it.value()->key;
        saveArr.append(obj);
    }
    saveObj["users"]=saveArr;
    QJsonDocument doc;
    doc.setObject(saveObj);
    QAesWrap aes(KEY,KEY,QAesWrap::AES_256);
    file.write(aes.encrypt(doc.toJson(),QAesWrap::AES_CTR));
    file.close();
}




