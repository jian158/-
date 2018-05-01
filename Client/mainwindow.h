#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "state.h"
#include <QtNetwork>
#include <QStandardItemModel>
#include "itemtable.h"
#include <QFile>
#include "friendlistview.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow //主窗口
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initView();
    void initServer(int TcpPort,int udpPort);
    void setUser(const QString &userName);
    void initFriendsList(const QJsonArray& friends);
    void linkServer(const QString& IP,const QString &port);
    void login(const QString &userName,const QString &passd);
    void loginSucceed(const QString &userName);
    void signIn(const QString &userName,const QString &passd,const QString &key);
    void findPassd(const QString &userName,const QString &key);
    void closeSokets();
    void addconnect(QTcpSocket *socket);
    void setMessage(QVector<Message>& msgList);
    void appenMessage(Message msg);
    void clearMessage();
    void switchUser();
    ~MainWindow();

public:
    Ui::MainWindow *ui;
    State state;
    QTcpSocket *linkServerSocket=nullptr;
    QTcpServer *server=nullptr;
    QUdpSocket *udpSocket=nullptr;

    QMap<QString,FriendObj*> friendList;
    QMap<int,QTcpSocket*> clients;

    QMap<int,ReceiveFileClass*> sendFileMap;
    QMap<int,ReceiveFileClass*> receiFileMap;

    QString currentfreind="";
    FriendListView *listview;
    QString fileDir="";
    int winSize=1;
public slots:
    void login();
    void sign();
    void findPassd();
    void linkServer();
    void changedSate(QAbstractSocket::SocketState);
    void disconnectServer();
    void linkError(QAbstractSocket::SocketError);
    void linkSucceed();
    void readServerMessage();
    void readMessage();
    void readclient();
    void itemClick(QModelIndex);
    void newLink();
    void readFile();
   void LogOut();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
signals:
    void loginSucceed();
};

#endif // MAINWINDOW_H
