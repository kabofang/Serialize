#ifndef _CSTORESTRATEGY_H_
#define _CSTORESTRATEGY_H_

class CStoreStrategy{
public:
        virtual bool Store(void *arg,int len) = 0;
        virtual bool Load(void *arg,int *plen) = 0;
};

#endif