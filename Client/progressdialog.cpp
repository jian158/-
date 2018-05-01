#include "progressdialog.h"
#include "ui_progressdialog.h"
#include "mainwindow.h"
ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    this->parennt=parent;
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    connect(this,SIGNAL(updateProgress(int)),this,SLOT(setProgressSlot(int)));
}

void ProgressDialog::setProgress(int value)
{
    ui->progressBar->setValue(value);
}



void ProgressDialog::setProgressInThread(int value)
{
    emit updateProgress(value);
}

void ProgressDialog::setText(const QString &text)
{
    ui->label->setText(text);
}

bool ProgressDialog::isStop()
{
    return this->isStoped;
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::setProgressSlot(int value)
{
    ui->progressBar->setValue(value);
}


