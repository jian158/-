#include "friendlistview.h"
#include <QHBoxLayout>

FriendListView::FriendListView(QWidget *parent):QListWidget(parent)
{

}

void FriendListView::setMsg(int row, int msgNum)
{
    dataList[row].msg->setText(QString::number(msgNum));
    dataList[row].msg->show();
}

void FriendListView::append(QString icon, QString Name, QString msg)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    Item item;
    item.icon=new QLabel;
    item.icon->setMaximumWidth(60);
    QPixmap pmp=QPixmap(icon);
    item.icon->setScaledContents(true);
    item.icon->setPixmap(pmp);

    item.Name=new QLabel(Name);
    item.msg=new QLabel(msg);

    item.msg->setMaximumWidth(54);
    item.msg->setAlignment(Qt::AlignCenter);
    item.msg->setStyleSheet("QLabel{border-image:url(:/icon/icon/new.png)}");
    item.msg->hide();
    layout->addWidget(item.icon);
    layout->addWidget(item.Name);
    layout->addWidget(item.msg);

    widget->setLayout(layout);
    QListWidgetItem *aItem = new QListWidgetItem(this);
    this->addItem(aItem);
    this->setItemWidget(aItem,widget);
    aItem->setSizeHint(QSize(0,80));
    
    widget->show();
    dataList.push_back(item);
}

QString FriendListView::getName(int row)
{
    return dataList[row].Name->text();
}


void FriendListView::hideMsg(int row)
{
    dataList[row].msg->hide();
}

void FriendListView::clear()
{
    dataList.clear();
    QListWidget::clear();
}


