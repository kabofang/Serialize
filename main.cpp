#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string.h>
using namespace std;
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
public:
	CA() { m_i=0;}
	explicit CA(int x) : m_i(x) {}
	
	void SetData(int x) {m_i = x;}
	int GetData(void) const{ return m_i; }
	
	int Serialize(char *pbuf) const;
	pCS DeSerialize(char *pbuf,int *plen);

	bool GetType(int& type);
};
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
class CB : public CSerializable{
private:
	double m_d;
public:
	CB() { m_d=0;}
	explicit CB(double x) : m_d(x) {}
	
	void SetData(double x) {m_d = x;}
	double GetData(void) const{ return m_d; }
	
	int Serialize(char *pbuf) const;
	pCS DeSerialize(char *pbuf,int *plen);

	bool GetType(int& type);
};
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
		memmove(pbuf,&type,sizeof(type));
		len += sizeof(type);
		pbuf += sizeof(type);
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
		memmove(&type1,pbuf,sizeof(type1));
		pbuf += sizeof(type1);
		len += sizeof(type1);
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
	CB b;

	pCS pa1 = new CA(5);
	pCS pb1 = new CB(3.14);
	
	
	VecSrc.push_back(pa1);
	VecSrc.push_back(pb1);
	
	//CSerializer Ser(new CStoreFileStrategy("a.txt"));
	CSerializer Ser(new CStoreFileStrategy("a.json"));

	Ser.Register(&a);
	Ser.Register(&b);

	Ser.Serialize(VecSrc);
	Ser.DeSerialize(Vecdst);
	
	for(int i = 0; i < Vecdst.size() ; i++){
		CA* pa = dynamic_cast<CA*>(Vecdst[i]);
		if(pa != NULL)
			cout<<pa->GetData()<<endl;
		CB* pb = dynamic_cast<CB*>(Vecdst[i]);
		if(pb != NULL)
			cout<<pb->GetData()<<endl;
	}
		
	return 0;
}
