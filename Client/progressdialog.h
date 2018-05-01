#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QMutex>

namespace Ui {
class ProgressDialog;
}


class ProgressDialog : public QDialog  //传输文件进度窗口
{
    Q_OBJECT
public:
    enum Type{
        Send,
        Receive
    };

public:

    explicit ProgressDialog(QWidget *parent = 0);
    void setProgress(int value);
    void setProgressInThread(int value);
    void setText(const QString &text);
    bool isStop();
    ~ProgressDialog();
signals:
    void updateProgress(int value);
private slots:
    void setProgressSlot(int value);

private:
    QWidget *parennt;
    Ui::ProgressDialog *ui;
    bool isStoped=false;
    Type type;
    int port;
};

#endif // PROGRESSDIALOG_H
