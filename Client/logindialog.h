#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}
#include "mainwindow.h"
class LoginDialog : public QDialog //登录窗口
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    explicit LoginDialog(MainWindow *parent = 0);
    ~LoginDialog();
private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginDialog *ui;
    MainWindow *mw;
};

#endif // LOGINDIALOG_H
