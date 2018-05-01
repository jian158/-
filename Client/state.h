#ifndef STATE_H
#define STATE_H
#include<QString>

class State   //用户状态
{
public:
    State();
    void setLogin(bool isLogined);
    bool isLogin();
    const QString getUserName();
    void setUserName(const QString& Name);
    void setServer(bool b);
    bool isServer();
private:
    bool isServered;
    bool isLogined;
    QString UserName="";
};

#endif // STATE_H
