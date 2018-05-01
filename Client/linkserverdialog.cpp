#include "linkserverdialog.h"
#include "ui_linkserverdialog.h"
#include "QMessageBox"
LinkServerDialog::LinkServerDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::LinkServerDialog)
{
    this->mw=parent;
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(linkServer()));
}

LinkServerDialog::~LinkServerDialog()
{
    delete ui;
}

void LinkServerDialog::linkServer()
{
    mw->linkServer(ui->lineEdit->text(),ui->lineEdit_2->text());
}


