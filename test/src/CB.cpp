#include "CB.h"
#include <string.h>
#include <iostream>
using namespace std;
CB::CB(int x,int y,double z,int v,double w){ 
         m_i1 = x;
         m_i2 = y;
         m_d = z;
         m_st.st_i = v;
         m_st.st_d = w;
}
void CB::DispData(void) const{
        cout<<m_i1<<"\t"<<m_i2<<"\t"<<m_d<<endl;
}
bool CB::GetType(int& type){
        type = 1;
        return true;
}
int CB::Serialize(char *pbuf) const{
        memmove(pbuf,(char*)this+8,sizeof(CB)-8);
        return sizeof(CB) - 8;
}
pCS CB::DeSerialize(char* pbuf,int *plen){
        *plen = sizeof(CB) -8;
        CB* pTemp = new CB;
        memmove((char*)pTemp+8,pbuf,sizeof(CB)-8);
        return pTemp;
}