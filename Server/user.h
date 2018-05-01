#ifndef USER_H
#define USER_H
#include <QString>
#include <QDebug>
#include <QMap>
typedef struct SaveUser{
    QString user;
    QString passd;
    QString key;
}SaveUser;

typedef struct TransTable{
    QString user;
    QString contact;
    QString data;
}TransTable;


class User
{
public:
    User();
    static void updataTransMap(QVector<TransTable>& transMap);
    static void readMessage(QVector<TransTable> &list);
    static void readFriends(QMap<QString,QJsonArray> &map);
    static void readUserList(QMap<QString,SaveUser*> &list);
    static void SaveUserList(QMap<QString,SaveUser*> &list);
    static void updataFriendsMap(QMap<QString,QJsonArray> &map);
private:
    static void createPath();
};

#endif // USER_H
