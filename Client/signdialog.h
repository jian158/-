#ifndef SIGNDIALOG_H
#define SIGNDIALOG_H

#include <QDialog>

namespace Ui {
class SignDialog;
}
#include "mainwindow.h"
class SignDialog : public QDialog //注册窗口
{
    Q_OBJECT

public:
    explicit SignDialog(MainWindow *parent = 0);
    ~SignDialog();
    void paintEvent(QPaintEvent *event) override;


private slots:
    void on_pushButton_clicked();

private:
    MainWindow *mw;
    Ui::SignDialog *ui;
};

#endif // SIGNDIALOG_H
