#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
LoginDialog::LoginDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::LoginDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->mw=parent;
    connect(parent,SIGNAL(loginSucceed()),this,SLOT(close()));
}



LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_clicked()
{
    if(!mw->state.isServer()){
        QMessageBox::information(this,"提示","服务器未连接！");
        return;
    }
    mw->login(ui->lineEdit->text(),ui->lineEdit_2->text());
}
