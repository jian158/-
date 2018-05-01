#ifndef FINDPASSDDIALOG_H
#define FINDPASSDDIALOG_H

#include <QDialog>

namespace Ui {
class FindPassdDialog;
}
#include "mainwindow.h"
class FindPassdDialog : public QDialog //找回密码模块
{
    Q_OBJECT

public:
    explicit FindPassdDialog(MainWindow *parent = 0);
    ~FindPassdDialog();
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_pushButton_clicked();

private:
    MainWindow *mw;
    Ui::FindPassdDialog *ui;
};

#endif // FINDPASSDDIALOG_H
