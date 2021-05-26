#ifndef _CSTOREFILESTRATEGY_H_
#define _CSTOREFILESTRATEGY_H_
#include "CStoreStrategy.h"

class CStoreFileStrategy:public CStoreStrategy{
private:
        char *pFilePath;
public:
        bool Store(void *arg,int len);
        bool Load(void *arg,int *plen);
        CStoreFileStrategy(const char*);
};

#endif