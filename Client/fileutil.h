#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <QMap>
#include "itemtable.h"
//用于读写聊天记录、配置信息类
class FileUtil
{
public:
    FileUtil();
    static void readMessage(const QString &userName,QMap<QString,FriendObj*> map);
    static void saveMsg(const QString &userName,QMap<QString,FriendObj*> map);
    static void ReadConf(QString& fileDir,int &WinSize);
    static void SaveConf(const QString& fileDir,const int &WinSize);
private:
    static void createPath();
};

#endif // FILEUTIL_H
