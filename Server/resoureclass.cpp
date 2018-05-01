#include "resoureclass.h"

ResoureClass::ResoureClass()
{

}

ResoureClass::ResoureClass(QTcpSocket *socket, int rowPosition){
    this->socket=socket;
    this->rowPosition=rowPosition;
}
