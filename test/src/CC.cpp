#include "CC.h"
#include <jsoncpp/json/json.h>
#include <iostream>
#include <string>
using namespace std;
CC::CC(int x,double y,double z,int v,double w){
         m_i = x;
         m_d1 = y;
         m_d2 = z;
         m_st.st_i = v;
         m_st.st_d = w;
}

void CC::DispData(void) const{
        cout<<m_i<<"\t"<<m_d1<<"\t"<<m_d2<<"\t"<<m_st.st_i<<"\t"<<m_st.st_d<<endl;
}

bool CC::GetType(int& type){
        type = 0;
        return true;
}
int CC::Serialize(char *pbuf) const{
        Json::Value CCobj;
        CCobj["m_i"] = Json::Value(m_i);
        CCobj["m_d1"] = Json::Value(m_d1);
        CCobj["m_d2"] = Json::Value(m_d2);
        Json::Value St;
        St["st_i"] = Json::Value(m_st.st_i);
        St["st_d"] = Json::Value(m_st.st_d);
        CCobj["m_st"] = Json::Value(St);
        Json::StyledWriter sw;
        string strtemp = sw.write(CCobj);
        int len = strtemp.size();
        strtemp.copy(pbuf,len,0);
        return len;
}
pCS CC::DeSerialize(char* pbuf,int *plen){
        Json::Reader rd;
        Json::Value CCobj;
        string temp = pbuf;
        rd.parse(temp,CCobj);
        int m_i = CCobj["m_i"].asInt();
        double m_d1 = CCobj["m_d1"].asDouble();
        double m_d2 = CCobj["m_d2"].asDouble();
        int m_st_st_i = CCobj["m_st"]["st_i"].asInt();
        double m_st_st_d = CCobj["m_st"]["st_d"].asInt();
        Json::StyledWriter sw;
        string tt = sw.write(CCobj);
        *plen = tt.size();
        CC* pTemp = new CC(m_i,m_d1,m_d2,m_st_st_i,m_st_st_d);
        return pTemp;
}
