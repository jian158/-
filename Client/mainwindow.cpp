#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "signdialog.h"
#include "findpassddialog.h"
#include "linkserverdialog.h"
#include <QMessageBox>
#include "handler.h"
#include "fileutil.h"
#include "timeutil.h"
#include <QFileDialog>
#include "log.h"
#include <QScrollBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QtConcurrent>
#include "filesr.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initView();
}


void MainWindow::initView(){
    listview=new FriendListView(ui->centralWidget);
    listview->setGeometry(0,0,291,611);
    connect(listview,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClick(QModelIndex)));

    ui->pushButton->setEnabled(false);
    QAction *a1=new QAction("登录");
    connect(a1,SIGNAL(triggered(bool)),this,SLOT(login()));
    ui->menu->addAction(a1);
    QAction *a2=new QAction("注册");
    ui->menu->addAction(a2);
    QAction *a3=new QAction("找回密码");
    ui->menu->addAction(a3);

    connect(a2,SIGNAL(triggered()),this,SLOT(sign()));
    connect(a3,SIGNAL(triggered()),this,SLOT(findPassd()));

    QAction *a4=new QAction("连接");
    ui->menu_2->addAction(a4);
    connect(a4,SIGNAL(triggered()),this,SLOT(linkServer()));
    FileUtil::ReadConf(fileDir,winSize);
}

void MainWindow::initServer(int TcpPort, int udpPort)
{
    emit loginSucceed();
    server=new QTcpServer();
    server->listen(QHostAddress::AnyIPv4,TcpPort);
    connect(server,SIGNAL(newConnection()),this,SLOT(newLink()));
    udpSocket=new QUdpSocket(this);
    udpSocket->bind(udpPort);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(readFile()));
}

void MainWindow::initFriendsList(const QJsonArray &friends){
    for (int i = 0; i < friends.size(); ++i) {
        QJsonObject fo=friends[i].toObject();
        QString _friend=fo["user"].toString();
        friendList[_friend]=new FriendObj("",_friend,i);
        listview->append(QString(":/icon/icon/head%1.png").arg(i%3+1),_friend);
    }
    FileUtil::readMessage(this->state.getUserName(),friendList);
}

void MainWindow::setUser(const QString &userName){
    this->state.setUserName(userName);
}

void MainWindow::itemClick(QModelIndex index){
    QString friend_=listview->getName(index.row());
    listview->hideMsg(index.row());
    this->currentfreind=friend_;
    FriendObj *item=friendList[friend_];
    item->msgSum=0;
    this->setMessage(item->text);
    ui->listWidget->verticalScrollBar()->setValue(ui->listWidget->verticalScrollBar()->maximum());
    if(item->socket==nullptr&&item->linking!=true){
        item->linking=true;
        QJsonObject sendObj;
        sendObj["what"]=Handler::LINKFRIEND;
        sendObj["friend"]=friend_;
        Handler::sendMsg(linkServerSocket,sendObj);
    }
}

void MainWindow::newLink()
{
    QTcpSocket *socket=server->nextPendingConnection();
    clients[socket->peerPort()]=socket;
    connect(socket,SIGNAL(readyRead()),this,SLOT(readclient()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectServer()));
    QJsonObject request;
    request["what"]=Handler::GETFRIENDINFO;
    request["user"]=state.getUserName();
    Handler::sendMsg(socket,request);
}

void MainWindow::readFile()
{
       int type;
        while(udpSocket->hasPendingDatagrams()){
            QByteArray datagram;
            datagram.resize(udpSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            udpSocket->readDatagram(datagram.data(), datagram.size(),&sender,&senderPort);

            QDataStream ds(&datagram,QIODevice::ReadOnly);
            ds>>type;

            if(type==Handler::newFile){
                newFile(this,udpSocket,sender,senderPort,ds,receiFileMap);
            }

            else if(type==Handler::endSendFile){
                endSendFile(this,sendFileMap,senderPort);
            }

            else if(type==Handler::SENDFILE){
                sendFile(udpSocket,sender,senderPort,winSize,ds,sendFileMap);
            }

            else if(type==Handler::endReceiveFile){
                endReceiveFile(this,udpSocket,receiFileMap,sender,senderPort);
            }

            else if(type==Handler::RECEIVEFILE){
                ReceiveFile(udpSocket,sender,senderPort,ds,receiFileMap,winSize);
            }
}
}

void MainWindow::login(const QString &userName, const QString &passd){
    QJsonObject sendObj;
    sendObj["what"]=Handler::LOGIN;
    sendObj["user"]=userName;
    sendObj["passd"]=passd;
    Handler::sendMsg(linkServerSocket,sendObj);
}

void MainWindow::loginSucceed(const QString &userName)
{
    this->setWindowTitle(userName);
    this->setUser(userName);
    QAction *remove=ui->menu->actions().at(0);
    ui->menu->removeAction(remove);
    QAction *firstAc=ui->menu->actions().at(0);
    QAction *userAc=new QAction(QIcon(":/icon/icon/user.png"),userName);
    ui->menu->insertAction(firstAc,userAc);


    QAction *logOut=new QAction("注销");
    ui->menu->addAction(logOut);
    connect(logOut,SIGNAL(triggered(bool)),this,SLOT(LogOut()));
}

void MainWindow::signIn(const QString &userName, const QString &passd, const QString &key)
{
    QJsonObject sendObj;
    sendObj["what"]=Handler::SIGNIN;
    sendObj["user"]=userName;
    sendObj["passd"]=passd;
    sendObj["key"]=key;
    Handler::sendMsg(linkServerSocket,sendObj);
}

void MainWindow::findPassd(const QString &userName, const QString &key)
{
    QJsonObject sendObj;
    sendObj["what"]=Handler::FINDPASSD;
    sendObj["user"]=userName;
    sendObj["key"]=key;
    Handler::sendMsg(linkServerSocket,sendObj);
}



void MainWindow::closeSokets()
{
    QMap<int,QTcpSocket*>::iterator it0=clients.begin();
    for (;it0!=clients.end();++it0) {
        if(it0.value()!=nullptr){
            it0.value()->close();
            it0.value()->deleteLater();
        }
    }

    QMap<QString,FriendObj*>::iterator it;
    for(it=friendList.begin();it!=friendList.end();++it){
        QTcpSocket *socket=it.value()->socket;
        if(socket!=nullptr){
            it.value()->socket->close();
            it.value()->socket->deleteLater();
            it.value()->socket=nullptr;
            it.value()->isLinked=false;
        }
    }
}

void MainWindow::addconnect(QTcpSocket *socket)
{
    connect(socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectServer()));
}

void MainWindow::setMessage(QVector<Message> &msgList)
{
    clearMessage();
    for (int i = 0; i < msgList.size(); ++i) {
        appenMessage(msgList[i]);
    }
}

void MainWindow::appenMessage(Message msg)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QLabel *lab1=new QLabel(msg.data); //显示消息控件
    lab1->setMinimumWidth(100);//设置气泡最小长度
    lab1->setMargin(40);//设置文字与距离边框
    if(msg.type==1){//等于1为好友的的消息
        //设置聊天气泡并置右边
       lab1->setStyleSheet("QLabel{border-image:url(:/icon/icon/chat3.png)}");
       layout->setAlignment(Qt::AlignRight);
    }
    else {//等于0为我的消息
        //设置聊天气泡并置左边
        lab1->setStyleSheet("QLabel{border-image:url(:/icon/icon/chat1.png);font-size:16}");
        layout->setAlignment(Qt::AlignLeft);
    }
    layout->addWidget(lab1);

    widget->setLayout(layout);
    QListWidgetItem *aItem = new QListWidgetItem(ui->listWidget);
    ui->listWidget->addItem(aItem);
    ui->listWidget->setItemWidget(aItem,widget);
    aItem->setSizeHint(QSize(0,80));
    widget->show();

    ui->listWidget->verticalScrollBar()->setValue(ui->listWidget->verticalScrollBar()->maximum());
}

void MainWindow::clearMessage()
{
    ui->listWidget->clear();
}

void MainWindow::LogOut()
{
    this->setWindowTitle("");
    QJsonObject logOutObj;
    logOutObj["what"]=Handler::LOGOUT;
    logOutObj["user"]=state.getUserName();
    Handler::sendMsg(linkServerSocket,logOutObj);
    switchUser();
    state.setLogin(false);
    state.setUserName("");
    QAction *remove=ui->menu->actions().at(0);
    ui->menu->removeAction(remove);

    QAction *firstAc=ui->menu->actions().at(0);
    QAction *a1=new QAction("登录");
    connect(a1,SIGNAL(triggered(bool)),this,SLOT(login()));
    ui->menu->insertAction(firstAc,a1);

    remove=ui->menu->actions().at(3);
    ui->menu->removeAction(remove);
}



void MainWindow::login(){
    switchUser();
    LoginDialog *d=new LoginDialog(this);
    d->setWindowTitle("切换用户");
    d->show();
}

void MainWindow::sign(){
    SignDialog *s=new SignDialog(this);
    s->show();
}

void MainWindow::findPassd(){
    FindPassdDialog *f=new FindPassdDialog(this);
    f->show();
}

void MainWindow::linkServer(){
    LinkServerDialog *l=new LinkServerDialog(this);
    l->show();
}

void MainWindow::changedSate(QAbstractSocket::SocketState state)
{
    closeSokets();
}

void MainWindow::disconnectServer(){
    qDebug()<<"6875496";
    QMap<QString,FriendObj*>::iterator it;
    for(it=friendList.begin();it!=friendList.end();++it){
        QTcpSocket *socket=it.value()->socket;
        if(socket!=nullptr&&socket->state()==QAbstractSocket::UnconnectedState){
            Log::LogInfo(state.getUserName()+"断开与"+it.value()->_friend+"的链接\n");
            it.value()->socket->close();
            it.value()->socket=nullptr;
            it.value()->isLinked=false;
        }
    }
    if(linkServerSocket->state()==QAbstractSocket::UnconnectedState){
        linkServerSocket->close();
    }

    QMap<int,QTcpSocket*>::iterator it1=clients.begin();
    for (;it1!=clients.end();++it1) {
        if(it1.value()->state()==QAbstractSocket::UnconnectedState){
            clients.remove(it1.key());
        }
    }
}


void MainWindow::linkServer(const QString& IP,const QString &port){
    linkServerSocket=new QTcpSocket();
    linkServerSocket->connectToHost(IP,port.toInt());
    connect(linkServerSocket,SIGNAL(disconnected()),this,SLOT(disconnectServer()));
    connect(linkServerSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(linkError(QAbstractSocket::SocketError)));
    connect(linkServerSocket,SIGNAL(connected()),this,SLOT(linkSucceed()));
    connect(linkServerSocket,SIGNAL(readyRead()),this,SLOT(readServerMessage()));
}

void MainWindow::linkSucceed(){
    state.setServer(true);
    QMessageBox::information(this,"提示","服务器连接成功",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void MainWindow::linkError(QAbstractSocket::SocketError err){
    state.setServer(false);
    state.setLogin(false);
    QMessageBox::information(this,"提示","服务器连接失败/断开",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void MainWindow::readMessage(){

    QMap<QString,FriendObj*>::iterator it1=friendList.begin();
    for(;it1!=friendList.end();++it1){
        QTcpSocket *socket=it1.value()->socket;
        if (socket!=nullptr&&socket->isReadable()&&socket->bytesAvailable()) {
            QByteArray arr=socket->readAll();
            QJsonParseError parseError;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(arr,&parseError));
            if(parseError.error==QJsonParseError::NoError&&jsonDoc.isObject()){
               QJsonObject obj=jsonDoc.object();
               if(obj.contains("what")){
                   Handler handler(this,obj);
               }
            }
        }
    }
}

void MainWindow::readclient()
{
    QMap<int,QTcpSocket*>::iterator it=clients.begin();
    for(;it!=clients.end();++it){
        QTcpSocket *socket=it.value();
        if (socket!=nullptr&&socket->isReadable()&&socket->bytesAvailable()) {
            QByteArray arr=socket->readAll();
            QJsonParseError parseError;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(arr,&parseError));
            if(parseError.error==QJsonParseError::NoError&&jsonDoc.isObject()){
               QJsonObject obj=jsonDoc.object();
               FriendObj *item=friendList[obj["user"].toString()];
               item->socket=it.value();
               item->udpport=obj["udp"].toString().toInt();
               item->isLinked=true;
            connect(item->socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
            connect(item->socket,SIGNAL(disconnected()),this,SLOT(disconnectServer()));
               clients.remove(it.key());
               break;
            }
        }
    }
}

void MainWindow::readServerMessage(){
    if(linkServerSocket->isReadable()&&linkServerSocket->bytesAvailable()){
        QByteArray arr=linkServerSocket->readAll();
        Log::LogInfo(state.getUserName()+"收到来自服务器的消息:"+arr+"\n");
        QJsonParseError parseError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(arr,&parseError));

        if(parseError.error==QJsonParseError::NoError&&jsonDoc.isObject()){
           QJsonObject obj=jsonDoc.object();
           if(obj.contains("what")){
               Handler handler(this,obj);
           }
        }

        else Log::LogInfo(state.getUserName()+"解析数据出错\n");
    }
}



void MainWindow::on_pushButton_clicked()
{
    if(currentfreind==""){
        QMessageBox::information(this,"提示","请选中好友",QMessageBox::Yes,QMessageBox::No);
                return;
    }
    FriendObj *item=friendList[currentfreind];
    QJsonObject sendObj;
    sendObj["user"]=this->state.getUserName();
    sendObj["what"]=Handler::SENDMSG;
    sendObj["d_account"]=item->_friend;
    QString chatData=ui->lineEdit->text();
    sendObj["data"]=chatData;
    Message msg(0,chatData);
    item->text.push_back(msg);
    this->appenMessage(msg);
    if(item->isLinked){
        Handler::sendMsg(item->socket,sendObj);
    }
    else {
        Handler::sendMsg(linkServerSocket,sendObj);
    }
    ui->lineEdit->setText("");
}


void MainWindow::on_pushButton_2_clicked()
{
    if(currentfreind==""){
        QMessageBox::information(this,"提示","请选中好友",QMessageBox::Yes,QMessageBox::No);
                return;
    }

    if(!friendList[currentfreind]->isLinked){
        QMessageBox::information(this,"提示","该好友不在线",QMessageBox::Yes,QMessageBox::No);
                return;
    }

    bool iscontains=sendFileMap.contains(friendList[currentfreind]->udpport);
    if(iscontains){
        QMessageBox::information(this,"提示","不能同时向一个人发送一个文件！",QMessageBox::Yes);
        return;
    }

    QString filePath=QFileDialog::getOpenFileName(this,"文件选择","./");
    if(filePath.length()==0)
        return;

    QFile *file=new QFile(filePath);
    if(!file->open(QIODevice::ReadOnly))
        return;
    QFileInfo info(*file);
    ReceiveFileClass *_class=new ReceiveFileClass;
    sendFileMap[friendList[currentfreind]->udpport]=_class;
    _class->file=file;
    _class->FileSize=file->size();
    _class->progess=new ProgressDialog(this);
    _class->progess->setText("正在向"+currentfreind+"发送"+info.fileName());
    _class->progess->show();
    QHostAddress *hostAddress=new QHostAddress(friendList[currentfreind]->socket->peerAddress());
    _class->timer.setSocket(udpSocket,hostAddress,friendList[currentfreind]->udpport);

    QByteArray *bytes=new QByteArray;
    QDataStream ds(bytes,QIODevice::WriteOnly);
    ds<<Handler::newFile<<info.fileName()<<file->size();
    _class->timer.sum=1;
    _class->timer.cache[0]={bytes,0};
    udpSocket->writeDatagram(*bytes,*hostAddress,friendList[currentfreind]->udpport);
    _class->timer.start(1000);
}


void MainWindow::switchUser()
{
    if(state.getUserName().length()>0)
        FileUtil::saveMsg(this->state.getUserName(),friendList);
    friendList.clear();

    if(server!=nullptr){
        server->close();
        server->deleteLater();
        server=nullptr;
    }

    closeSokets();
    if(udpSocket!=nullptr){
        udpSocket->close();
        udpSocket->deleteLater();
        udpSocket=nullptr;
    }
    listview->clear();
    ui->listWidget->clear();
    currentfreind=="";
    clients.clear();
}


MainWindow::~MainWindow()
{
    switchUser();
    FileUtil::SaveConf(fileDir,winSize);
    if(linkServerSocket!=nullptr&&linkServerSocket->ConnectedState==QAbstractSocket::UnconnectedState)
        linkServerSocket->close();
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug()<<"change";
    if(ui->lineEdit->text().length()>0)
        ui->pushButton->setEnabled(true);
    else  ui->pushButton->setEnabled(false);
}

