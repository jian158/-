#include "findpassddialog.h"
#include "ui_findpassddialog.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPaintEvent>
#include <QMessageBox>
FindPassdDialog::FindPassdDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::FindPassdDialog)
{
    ui->setupUi(this);
    this->mw=parent;
}

FindPassdDialog::~FindPassdDialog()
{
    delete ui;
}

void FindPassdDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen());
    painter.setBrush(QBrush());
    painter.fillRect(event->rect(),Qt::white);
}

void FindPassdDialog::on_pushButton_clicked()
{
    if(!mw->state.isServer()){
        QMessageBox::information(this,"提示","服务器未连接！");
        return;
    }
    mw->findPassd(ui->lineEdit->text(),ui->lineEdit_2->text());
}
