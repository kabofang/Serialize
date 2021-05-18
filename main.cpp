#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
using namespace std;
class CSerializable{
public:
	virtual bool Serialize(int fd) const = 0;
	virtual CSerializable* DeSerialize(int fd) = 0;
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
	int GetData(void) { return m_i; }
	
	bool Serialize(int fd) const;
	pCS DeSerialize(int fd);

	bool GetType(int& type);
};
bool CA::GetType(int& type){
	type = 0;
	return true;
}
bool CA::Serialize(int fd) const{
	if(-1 == ::write(fd,&m_i,sizeof(m_i))){
		cout<<"Write file fail!"<<endl;
		return false;
	}
	return true;
}
pCS CA::DeSerialize(int fd){
	CA* pTemp = new CA;
	int ret = ::read(fd,&(pTemp->m_i),sizeof(int));
	if(-1 == ret || 0 == ret)
		return NULL;
	return pTemp;
}

class CB : public CSerializable{
private:
	double m_d;
public:
	CB() { m_d=0;}
	explicit CB(double x) : m_d(x) {}

	void SetData(double x) {m_d = x;}
	double GetData(void) { return m_d; }

	bool Serialize(int fd) const;
	pCS DeSerialize(int fd);
	bool GetType(int& type);
};
bool CB::GetType(int& type){
	type = 1;
	return true;
}
bool CB::Serialize(int fd) const{
	if(-1 == ::write(fd,&m_d,sizeof(m_d))){
		cout<<"Write file fail!"<<endl;
		return false;
	}
	return true;
}
pCS CB::DeSerialize(int fd){
	CB* pTemp = new CB;
	int ret = ::read(fd,&(pTemp->m_d),sizeof(m_d));
	if(-1 == ret || 0 == ret)
		return NULL;
	return pTemp;
}


class CC : public CSerializable{
private:
	int m_i;
public:
	CC() { m_i=0;}
	explicit CC(int x) : m_i(x) {}

	void SetData(int x) {m_i = x;}
	double GetData(void) { return m_i; }

	bool Serialize(int fd) const;
	pCS DeSerialize(int fd);
	bool GetType(int& type);
};
bool CC::GetType(int& type){
	type = 2;
	return true;
}

bool CC::Serialize(int fd) const{
	if(-1 == ::write(fd,&m_i,sizeof(m_i))){
		cout<<"Write file fail!"<<endl;
		return false;
	}
	return true;
}
pCS CC::DeSerialize(int fd){
	CC* pTemp = new CC;
	int ret = ::read(fd,&(pTemp->m_i),sizeof(m_i));
	if(-1 == ret || 0 == ret)
		return NULL;
	return pTemp;
}

class CSerializer{
private:
	vector<pCS> m_Vecreg;
public://问老师static与之区别
	bool Serialize(const char* pFilePath , const vector<pCS> &Vec) const;
	bool DeSerialize(const char* pFilePath , vector<pCS> &Vec);
	bool Register(pCS p);
};

bool CSerializer::Serialize(const char* pFilePath , const vector<pCS> &Vec) const{
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_CREAT|O_TRUNC|O_WRONLY,00600))){
		cout << "Open file fail!" << endl;
		return false;
	}
	for(int i = 0 ; i < Vec.size() ; i++){
		int type;
		Vec[i]->GetType(type);
		write(fd,&type,sizeof(type));
		Vec[i]->Serialize(fd);
	}

	if(-1 == ::close(fd)){
		cout << "Close file fail!" << endl;
		return false;
	}
	return true;
}
bool CSerializer::DeSerialize(const char* pFilePath , vector<pCS> &Vec){	
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_RDONLY))){
		cout << "Open file fail!" << endl;
		return false;
	}

	for(;;){
		int type1,type2;
		pCS pTemp;
		if(0 == read(fd,&type1,sizeof(type1)))
			break;
		for(int i = 0;i < m_Vecreg.size();i++){
			m_Vecreg[i]->GetType(type2);
			if(type1 == type2){
				pTemp = m_Vecreg[i]->DeSerialize(fd);
				Vec.push_back(pTemp);
			}
		}
		
	}

	if(-1 == ::close(fd)){
		cout << "Close file fail!" << endl;
		return false;
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
	CC c;


	pCS pa1 = new CA(5);
	pCS pb1 = new CB(6);
	pCS pc1 = new CC(7);
	
	
	VecSrc.push_back(pa1);
	VecSrc.push_back(pb1);
	VecSrc.push_back(pc1);
	

	CSerializer Ser;

	Ser.Register(&a);
	Ser.Register(&b);
	Ser.Register(&c);

	Ser.Serialize("a.txt",VecSrc);
	Ser.DeSerialize("a.txt",Vecdst);
	
	for(int i = 0; i < Vecdst.size() ; i++){
		CA* pa = dynamic_cast<CA*>(Vecdst[i]);
		if(pa != NULL)
			cout<<pa->GetData()<<endl;
		CB* pb = dynamic_cast<CB*>(Vecdst[i]);
		if(pb != NULL)
			cout<<pb->GetData()<<endl;
		CC* pc = dynamic_cast<CC*>(Vecdst[i]);
		if(pc != NULL)
			cout<<pc->GetData()<<endl;
	
	}
		
	return 0;
}
