#include "CA.h"
#include <string.h>
#include <iostream>
using namespace std;
CA::CA(int x,double y,double z,int v,double w){
    m_i = x;
    m_d1 = y;
    m_d2 = z;
    m_st.st_i = v;
    m_st.st_d = w;
}
void CA::DispData(void) const{
    cout<<m_i<<"\t"<<m_d1<<"\t"<<m_d2<<endl;
}
    
bool CA::GetType(int& type){
    type = 0;
    return true;
}
int CA::Serialize(char *pbuf) const{
    memmove(pbuf,(char*)this+8,sizeof(CA)-8);
    return sizeof(CA) - 8;
}
pCS CA::DeSerialize(char* pbuf,int *plen){
    *plen = sizeof(CA) -8; 
    CA* pTemp = new CA; 
    memmove((char*)pTemp+8,pbuf,sizeof(CA)-8);
    return pTemp;
}
