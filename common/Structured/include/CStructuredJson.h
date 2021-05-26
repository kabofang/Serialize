#ifndef _CSTRUCTUREDJSON_H_
#define _CSTRUCTUREDJSON_H_

#include "CStoreByTypeAndReg.h"

class CStructuredJson:public CStoreByTypeAndReg{
public:
    int WriteType(char*pbuf,int type) const;
    int ReadType(char*pbuf,int *ptype) const;
    CStructuredJson(CStoreStrategy *p);
    ~CStructuredJson();
};

#endif