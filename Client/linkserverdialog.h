#pragma once
#ifndef LINKSERVERDIALOG_H
#define LINKSERVERDIALOG_H
#include <QDialog>
namespace Ui {
class LinkServerDialog;
}

#include "mainwindow.h"

class LinkServerDialog : public QDialog  //连接服务器窗口
{
    Q_OBJECT

public:
    explicit LinkServerDialog(MainWindow *parent = 0);
    ~LinkServerDialog();

private slots:
    void linkServer();

private:
    MainWindow *mw;
    Ui::LinkServerDialog *ui;
};

#endif // LINKSERVERDIALOG_H
