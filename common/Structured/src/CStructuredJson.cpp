#include "CStructuredJson.h"
#include <jsoncpp/json/json.h>
CStructuredJson::CStructuredJson(CStoreStrategy *p){
    m_Strategy = p;
}

CStructuredJson::~CStructuredJson(){
    delete m_Strategy;
}

int CStructuredJson::WriteType(char*pbuf,int type) const{
    Json::Value typej;
    typej["type"] = Json::Value(type);
    Json::StyledWriter sw; 
    string temp = sw.write(typej);
    int jsonlen = temp.size();
    temp.copy(pbuf,jsonlen,0);
    return jsonlen;
}

int CStructuredJson::ReadType(char*pbuf,int *ptype) const{
    Json::Value typej;
    Json::Reader rd;
    string strtemp = pbuf;
    rd.parse(strtemp,typej);
    *ptype = typej["type"].asInt();
    Json::StyledWriter sw;
    string jstype = sw.write(typej);
    int templen = jstype.size();
    return templen;
}