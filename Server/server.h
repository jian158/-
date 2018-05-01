#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include<QTcpSocket>
#include <QTcpServer>
#include<QHostAddress>
#include <QDebug>
#include <QString>
#include <QDataStream>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMap>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <resoureclass.h>
#include <QVector>
#include "user.h"
#include <QSet>
namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    void initView();
    void initData();
    void setUserName(QString Name,int rowIndex,int udpPort,int tcpPort);
    void setUserName(QString Name,int rowIndex,const QString& udpPort,const QString &tcpPort);
    int getcreatePort();
    ~Server();
private slots:
    void newLink();
    void Read();
    void disLink();

    void on_pushButton_clicked();

private:
    QTcpServer *server;
    QStandardItemModel *model;
public:
    QVector<ResoureClass*> socketMap;
    QMap<QString,QJsonArray> friendMap;
    QVector<TransTable> transMap;
    QMap<QString,SaveUser*> userList;
    QSet<int> portSet;
    Ui::Server *ui;
};

#endif // SERVER_H
