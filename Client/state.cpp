#include "state.h"

State::State()
{
    isLogined=false;
    isServered=false;
    UserName="";
}
void State::setLogin(bool isLogined){
    this->isLogined=isLogined;
}

bool State::isLogin(){
    return this->isLogined;
}

const QString State::getUserName()
{
    return this->UserName;
}

void State::setServer(bool b){
    this->isServered=b;
}

void State::setUserName(const QString& Name){
    this->UserName=Name;
}

bool State::isServer(){
    return this->isServered;
}
