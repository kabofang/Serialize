#include "CStoreByTypeAndReg.h"
#include <jsoncpp/json/json.h>
#include <string.h>
#include <vector>
using namespace std;

CStoreByTypeAndReg::CStoreByTypeAndReg(){
}

CStoreByTypeAndReg::~CStoreByTypeAndReg(){
}

bool CStoreByTypeAndReg::Serialize( const vector<pCS> &Vec) const{
    char buf[1024];
    int len = 0;
    char* pbuf = buf;
    memset(buf,0,1024);

    for(int i = 0 ;i < Vec.size(); i++){
            int type;
            Vec[i]->GetType(type);
            int entlen = WriteType(pbuf,type);
            len += entlen;
            pbuf += entlen;

            int templen =  Vec[i]->Serialize(pbuf);
            len += templen;
            pbuf += templen;
    }
    m_Strategy->Store(buf,len);
    
    return true;
}
bool CStoreByTypeAndReg::DeSerialize(vector<pCS> &Vec){
    char* pbuf;
    int originlen = 0;
    int len = 0;
    int Delen;
    m_Strategy->Load(&pbuf,&originlen);
    for(;len<originlen;){
            int type1,type2;
            pCS pTemp;
            int entlen = ReadType(pbuf,&type1);
            pbuf += entlen;
            len += entlen;

            for(int i = 0;i < m_Vecreg.size();i++){
                    m_Vecreg[i]->GetType(type2);
                    if(type1 == type2){
                            pTemp = m_Vecreg[i]->DeSerialize(pbuf,&Delen);
                            pbuf += Delen;
                            len += Delen;
                            Vec.push_back(pTemp);
                    }
            }
    }

    return true;
}
bool CStoreByTypeAndReg::Register(pCS p){
    m_Vecreg.push_back(p);
    return true;
}