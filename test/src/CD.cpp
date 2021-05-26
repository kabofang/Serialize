#include "CD.h"
#include <jsoncpp/json/json.h>
#include <iostream>
#include <string>
using namespace std;
CD::CD(int x,int y,double z,int v,double w){
         m_i1 = x;
         m_i2 = y;
         m_d = z;
         m_st.st_i = v;
         m_st.st_d = w;
}

void CD::DispData(void) const{
        cout<<m_i1<<"\t"<<m_i2<<"\t"<<m_d<<"\t"<<m_st.st_i<<"\t"<<m_st.st_d<<endl;
}

bool CD::GetType(int& type){
        type = 1;
        return true;
}

int CD::Serialize(char *pbuf) const{
        Json::Value CDobj;
        CDobj["m_i1"] = Json::Value(m_i1);
        CDobj["m_i2"] = Json::Value(m_i2);
        CDobj["m_d"] = Json::Value(m_d);
        Json::Value St;
        St["st_i"] = Json::Value(m_st.st_i);
        St["st_d"] = Json::Value(m_st.st_d);
        CDobj["m_st"] = Json::Value(St);
        Json::StyledWriter sw;
        string strtemp = sw.write(CDobj);
        //const char *charptemp = strtemp.c_str();
        int len = strtemp.size();
        strtemp.copy(pbuf,len,0);
        return len;
}

pCS CD::DeSerialize(char* pbuf,int *plen){
        Json::Reader rd;
        Json::Value CDobj;
        string temp = pbuf;
        rd.parse(temp,CDobj);
        int m_i1 = CDobj["m_i1"].asInt();
        int m_i2 = CDobj["m_i2"].asDouble();
        double m_d = CDobj["m_d"].asDouble();
        int m_st_st_i = CDobj["m_st"]["st_i"].asInt();
        double m_st_st_d = CDobj["m_st"]["st_d"].asInt();
        Json::StyledWriter sw;
        string tt = sw.write(CDobj);
        *plen = tt.size();
        CD* pTemp = new CD(m_i1,m_i2,m_d,m_st_st_i,m_st_st_d);
        return pTemp;
}