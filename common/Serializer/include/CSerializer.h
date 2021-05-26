#ifndef _CSERIALIZER_H_
#define _CSERIALIZER_H_
#include "CStoreByTypeAndReg.h"

class CSerializer:public CStoreByTypeAndReg{
public:
    int WriteType(char*pbuf,int type) const;
    int ReadType(char*pbuf,int *ptype) const;
    CSerializer(CStoreStrategy *p);
    ~CSerializer();
};

#endif