#include "CSerializer.h"
#include <string.h>
CSerializer::CSerializer(CStoreStrategy *p){
    m_Strategy = p;
}
CSerializer::~CSerializer(){
    delete m_Strategy;
}

int CSerializer::WriteType(char*pbuf,int type) const{
    memmove(pbuf,&type,sizeof(type));
    return sizeof(type);
}
int CSerializer::ReadType(char*pbuf,int *ptype) const{
    memmove(ptype,pbuf,sizeof(int));
    return sizeof(int);
}