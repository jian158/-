#ifndef FRIENDLISTVIEW_H
#define FRIENDLISTVIEW_H

#include <QObject>
#include <QListWidget>
#include <QLabel>
#include <QWidget>
class FriendListView:public QListWidget  //好友列表控件
{
    Q_OBJECT
    struct Item{//控件组
        QLabel *icon;//头像
        QLabel *Name;//用户名
        QLabel *msg; //聊天
    };

public:
    FriendListView(QWidget *parent);
    void append(QString icon,QString Name,QString msg="");//增加好友
    QString getName(int row);//获取好友名称
    void setMsg(int row,int msgNum);//设置消息通知
    void hideMsg(int row);//隐藏通知
    void clear();//清空
private:
    QVector<Item> dataList;
};

#endif // FRIENDLISTVIEW_H
