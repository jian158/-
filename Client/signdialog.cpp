#include "signdialog.h"
#include "ui_signdialog.h"
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPaintEvent>
SignDialog::SignDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::SignDialog)
{
    ui->setupUi(this);
    this->mw=parent;
}

SignDialog::~SignDialog()
{
    delete ui;
}

void SignDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen());
    painter.setBrush(QBrush());
    painter.fillRect(event->rect(),Qt::white);

}

void SignDialog::on_pushButton_clicked()
{
    if(!mw->state.isServer()){
        QMessageBox::information(this,"提示","服务器未连接！");
        return;
    }
    if(ui->lineEdit_2->text()!=ui->lineEdit_3->text()){
        QMessageBox::information(this,"提示","两次密码不一致");
        return;
    }

    mw->signIn(ui->lineEdit->text(),ui->lineEdit_2->text(),ui->lineEdit_4->text());
}
