#include "server.h"
#include "ui_server.h"
#include <QHeaderView>
#include <QAction>
#include <QByteArray>
#include <handler.h>
#include <QMessageBox>
Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    this->initData();
    this->initView();
    server=new QTcpServer(this);
}

void Server::Read(){
    for(int i=0;i<socketMap.size();++i){
        QTcpSocket *socket=socketMap[i]->socket;
        if (socket->isReadable()&&socket->bytesAvailable()) {
            QByteArray arr=socket->readAll();
            QJsonParseError parseError;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(arr,&parseError));
            if(parseError.error==QJsonParseError::NoError&&jsonDoc.isObject()){
               QJsonObject obj=jsonDoc.object();
               if(obj.contains("what")){
                   Handler handler(this,obj,socketMap[i]);
               }
            }
        }
    }
}

void Server::initView(){
    this->model=new QStandardItemModel();
    model->setColumnCount(3);
    model->setHorizontalHeaderItem(0,new QStandardItem("用户名"));
    model->setHorizontalHeaderItem(1,new QStandardItem("IP"));
    model->setHorizontalHeaderItem(2,new QStandardItem("tcp端口"));
    model->setHorizontalHeaderItem(3,new QStandardItem("udp端口"));
    ui->tableView->horizontalHeader()->setDefaultSectionSize(148);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->show();
}



void Server::initData()
{
    User::readFriends(friendMap);
    User::readMessage(transMap);
    User::readUserList(userList);
}


void Server::setUserName(QString Name,int rowIndex,int udpPort,int tcpPort){
    model->setItem(rowIndex,0,new QStandardItem(Name));
    model->setItem(rowIndex,2,new QStandardItem(QString::number(tcpPort)));
    model->setItem(rowIndex,3,new QStandardItem(QString::number(udpPort)));
}

void Server::setUserName(QString Name,int rowIndex,const QString& udpPort,const QString &tcpPort){
    model->setItem(rowIndex,0,new QStandardItem(Name));
    model->setItem(rowIndex,2,new QStandardItem(tcpPort));
    model->setItem(rowIndex,3,new QStandardItem(udpPort));
}

int Server::getcreatePort()
{
    qsrand(0);
    for (int i = 0; i < 10000; ++i) {
        int port=qrand()%20000+100;
        if(!portSet.contains(port)){
            portSet.insert(port);
            return port;
    }
    }
    return -1;
}


void Server::newLink(){
    QTcpSocket *socket=server->nextPendingConnection();
    QString address=socket->peerAddress().toString();
    model->setItem(socketMap.count(),1,new QStandardItem(address));

    socketMap.push_back(new ResoureClass(socket,socketMap.count()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(Read()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disLink()));
}

void Server::disLink(){
    for(int i=0;i<socketMap.size();){
        QTcpSocket *socket=socketMap[i]->socket;
        if(socket->state()==QAbstractSocket::UnconnectedState){
            model->removeRow(socketMap[i]->rowPosition);
            for (int j = i+1; j < socketMap.size(); ++j) {
                socketMap[j]->rowPosition--;
            }
            socketMap.remove(i);
        }
        else i++;
    }
}

Server::~Server()
{
    User::updataTransMap(transMap);
    User::updataFriendsMap(friendMap);
    User::SaveUserList(userList);
    server->close();
    delete ui;
}

void Server::on_pushButton_clicked()
{
    if(server->isListening()){
        QMessageBox::information(this,"提示","已经开启无需再开！");
        return;
    }

    short port=ui->lineEdit->text().toShort();
    if(!server->listen(QHostAddress::AnyIPv4,port)){
        QMessageBox::information(this,"提示","开启失败，请稍后再试！");
        return;
    }
    ui->pushButton->setText("已开启");
    ui->pushButton->setEnabled(false);
    QMessageBox::information(this,"提示","开启成功");
    connect(server,SIGNAL(newConnection()),this,SLOT(newLink()));
}
