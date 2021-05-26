#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string.h>
#include <jsoncpp/json/json.h>
using namespace std;
struct TestData{
	int st_i;
	double st_d;
};
class CStoreStrategy{
public: 
	virtual bool Store(void *arg,int len) = 0;
	virtual bool Load(void *arg,int *plen) = 0;
};
class CStoreJsonFileStrategy:public CStoreStrategy{
public:
	bool Store(void *arg,int len);
	bool Load(void *arg,int *plen);
};
class CStoreFileStrategy:public CStoreStrategy{
private:
	char *pFilePath;
public: 
	bool Store(void *arg,int len);
	bool Load(void *arg,int *plen); 
	CStoreFileStrategy(const char*);
};
CStoreFileStrategy::CStoreFileStrategy(const char*p){
	pFilePath = new char[strlen(p)+1];
	memmove(pFilePath,p,strlen(p)+1);
	return ;
}
bool CStoreFileStrategy::Store(void *arg,int len){
	char *pbuf = (char*)arg;
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_CREAT|O_TRUNC|O_WRONLY,00600))){
		cout << "Open file fail!" << endl;
		return false;
	}
	::write(fd,pbuf,len);
	if(-1 == ::close(fd)){
		cout << "Close file fail!" << endl;
		return false;
	}
	return true;
}
bool CStoreFileStrategy::Load(void *arg,int *plen){
	char **ppbuf = (char**)arg;
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_RDONLY))){
		cout << "Open file fail!" << endl;
		return false;
	}
	int filesize = lseek(fd,0,SEEK_END);
	*plen = filesize;
	*ppbuf = (char*)malloc(filesize);
	lseek(fd,0,SEEK_SET);
	::read(fd,*ppbuf,filesize);
	if(-1 == ::close(fd)){
		cout << "Close file fail!" << endl;
		return false;
	}
	return true;
}


class CSerializable{
public:
	virtual int Serialize(char *pbuf) const = 0;
	virtual CSerializable* DeSerialize(char *pbuf,int *plen) = 0;
	virtual bool GetType(int& type) = 0;
};

typedef CSerializable* pCS;

class CA : public CSerializable{
private:
	int m_i;
	double m_d1;
	double m_d2;
	TestData m_st;
public:
	explicit CA(int x=0,double y=0,double z=0,int v=0,double w=0);
	//void SetData(int x) {m_i = x;}
	void DispData(void) const;
	
	int Serialize(char *pbuf) const;
	pCS DeSerialize(char *pbuf,int *plen);

	bool GetType(int& type);
};
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
	Json::Value CAobj;
	CAobj["m_i"] = Json::Value(m_i);
	CAobj["m_d1"] = Json::Value(m_d1);
	CAobj["m_d2"] = Json::Value(m_d2);
	Json::Value St;
	St["st_i"] = Json::Value(m_st.st_i);
	St["st_d"] = Json::Value(m_st.st_d);
	CAobj["m_st"] = Json::Value(St);
	Json::StyledWriter sw;
	string strtemp = sw.write(CAobj);
	const char *charptemp = strtemp.c_str();
	int len = strlen(charptemp)+1;
	memmove(pbuf,charptemp,len);
	return len;
}
pCS CA::DeSerialize(char* pbuf,int *plen){
	Json::Reader rd;
	Json::Value CAobj;
	rd.parse(pbuf,CAobj);
	int m_i = CAobj["m_i"].asInt();
	double m_d1 = CAobj["m_d1"].asDouble();
	double m_d2 = CAobj["m_d2"].asDouble();
	int m_st_st_i = CAobj["m_st"]["st_i"].asInt();
	double m_st_st_d = CAobj["m_st"]["st_d"].asInt();
	*plen = strlen(pbuf)+1;
	CA* pTemp = new CA(m_i,m_d1,m_d2,m_st_st_i,m_st_st_d);
	return pTemp;
}
class CB : public CSerializable{
private:
	int m_i1;
	int m_i2;
	double m_d;
	TestData m_st;
public:
	explicit CB(int x=0,int y=0,double z=0,int v=0,double w=0);
	
	//void SetData(double x,) {m_d = x;}
	void DispData(void) const;
	
	int Serialize(char *pbuf) const;
	pCS DeSerialize(char *pbuf,int *plen);

	bool GetType(int& type);
};
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

class CStorage{
private:
	vector<pCS> m_Vecreg;
public:
	bool Serialize(const char* pFilePath , const vector<pCS> &Vec) const;
	bool DeSerialize(const char* pFilePath , vector<pCS> &Vec);
	bool Register(pCS p);
};

class CSerializer:public CStorage{
private:
	vector<pCS> m_Vecreg;
	char* m_StrTemp;
	CStoreStrategy *m_Strategy;
public:
	bool Serialize(const vector<pCS> &Vec) const;
	bool DeSerialize(vector<pCS> &Vec);
	bool Register(pCS p);
	
	CSerializer(CStoreStrategy *p):m_Strategy(p){}
};

bool CSerializer::Serialize(const vector<pCS> &Vec) const{
	char buf[1024];
	int len = 0;
	char* pbuf = buf;
	memset(buf,0,1024);

	for(int i = 0 ;i < Vec.size(); i++){
		int type;
		Vec[i]->GetType(type);

		Json::Value typej;
		typej["type"] = Json::Value(type);
		Json::StyledWriter sw;
		string temp = sw.write(typej);
		const char* charpstr = temp.c_str();
		int jsonlen = strlen(charpstr)+1;
		memmove(pbuf,charpstr,jsonlen);
		len += jsonlen;
		pbuf += jsonlen;

		int templen =  Vec[i]->Serialize(pbuf);
		len += templen;
		pbuf += templen;
	}
	m_Strategy->Store(buf,len);
	return true;
}
bool CSerializer::DeSerialize(vector<pCS> &Vec){	
	char* pbuf;
	int originlen = 0;
	int len = 0;
	int Delen;
	m_Strategy->Load(&pbuf,&originlen);
	for(;len<originlen;){
		int type1,type2;
		pCS pTemp;
		Json::Value typej;
		Json::Reader rd;
		string strtemp = pbuf;
		rd.parse(strtemp,typej);
		type1 = typej["type"].asInt();
		const char *charptemp = strtemp.c_str();
		int templen = strlen(charptemp)+1;
		pbuf += templen;
		len += templen;
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
bool CSerializer::Register(pCS p){
	m_Vecreg.push_back(p);
	return true;
}


int main(int argc , char* argv[]){
	vector<pCS> VecSrc;
	vector<pCS> Vecdst;

	CA a;
//	CB b;

	pCS pa1 = new CA(5,1.1,1.2,1,1);
//	pCS pb1 = new CB(6,7,3.14);
	
	
	VecSrc.push_back(pa1);
//	VecSrc.push_back(pb1);
	
	//CSerializer Ser(new CStoreFileStrategy("a.txt"));
	CSerializer Ser(new CStoreFileStrategy("a.json"));

	Ser.Register(&a);
//	Ser.Register(&b);

	Ser.Serialize(VecSrc);
	Ser.DeSerialize(Vecdst);
	
	for(int i = 0; i < Vecdst.size() ; i++){
		CA* pa = dynamic_cast<CA*>(Vecdst[i]);
		if(pa != NULL)
			pa->DispData();
/*		CB* pb = dynamic_cast<CB*>(Vecdst[i]);
		if(pb != NULL)
			pb->DispData();
*/	}
		
	return 0;
}
