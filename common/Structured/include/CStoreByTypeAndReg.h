#ifndef _CSTOREBYTYPEANDREG_H_
#define _CSTOREBYTYPEANDREG_H_

#include "CStorage.h"

class CStoreByTypeAndReg:public CStorage{
private:
        vector<pCS> m_Vecreg;
public:
        bool Serialize(const vector<pCS> &Vec) const;
        bool DeSerialize(vector<pCS> &Vec);
        virtual int ReadType(char*pbuf,int*ptype) const = 0;
        virtual int WriteType(char *pbuf,int type) const = 0;
        bool Register(pCS p);
        CStoreByTypeAndReg();
        virtual ~CStoreByTypeAndReg();
};

#endif