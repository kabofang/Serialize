#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
using namespace std;
struct StObj{
	int m_Type;
	void* m_pObj;
};
class CA{
private:
	int m_i;
public:
	CA() { m_i=0; }
	explicit CA(int x) : m_i(x) {}

	void SetData(int x) {m_i = x;}
	int GetData(void) { return m_i; }

	bool Serialize(int fd) const;
	bool DeSerialize(int fd);
};

bool CA::Serialize(int fd) const{
	if(-1 == ::write(fd,&m_i,sizeof(m_i))){
		cout<<"Write file fail!"<<endl;
		return false;
	}
	return true;
}
bool CA::DeSerialize(int fd){
	int ret = ::read(fd,&m_i,sizeof(m_i));
	if(-1 == ret || 0 == ret)
		return false;
	return true;
}

class CB{
private:
	double m_d;
public:
	CB() { m_d=0; }
	explicit CB(double x) : m_d(x) {}

	void SetData(double x) {m_d = x;}
	double GetData(void) { return m_d; }

	bool Serialize(int fd) const;
	bool DeSerialize(int fd);
};

bool CB::Serialize(int fd) const{
	if(-1 == ::write(fd,&m_d,sizeof(m_d))){
		cout<<"Write file fail!"<<endl;
		return false;
	}
	return true;
}
bool CB::DeSerialize(int fd){
	int ret = ::read(fd,&m_d,sizeof(m_d));
	if(-1 == ret || 0 == ret)
		return false;
	return true;
}


class CSerializer{
public://问老师static与之区别
	bool Serialize(const char* pFilePath , const vector<StObj> &Vec) const;
	bool DeSerialize(const char* pFilePath , vector<StObj> &Vec);
};

bool CSerializer::Serialize(const char* pFilePath , const vector<StObj> &Vec) const{
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_CREAT|O_TRUNC|O_WRONLY,00600))){
		cout << "Open file fail!" << endl;
		return false;
	}
	for(int i = 0 ; i < Vec.size() ; i++){
		if(-1 == ::write(fd,&(Vec[i].m_Type),sizeof(Vec[i].m_Type))){
			cout << "Write file fail!" << endl;
			return false;
		}

		if(1 == Vec[i].m_Type)
			if( !(((CA*)(Vec[i].m_pObj))->Serialize(fd)) ){
				cout << "Serialize fail!" << endl;
				return false;
			}	

		if(2 == Vec[i].m_Type)
			if( !(((CB*)(Vec[i].m_pObj))->Serialize(fd)) ){
				cout << "Serialize fail!" << endl;
				return false;
			}
	}

	if(-1 == ::close(fd)){
		cout << "Close file fail!" << endl;
		return false;
	}
	return true;
}
bool CSerializer::DeSerialize(const char* pFilePath , vector<StObj> &Vec){	
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_RDONLY))){
		cout << "Open file fail!" << endl;
		return false;
	}

	for(;;){
		StObj Temp;

		int ret;
		ret = ::read(fd,&Temp.m_Type,sizeof(Temp.m_Type));
		if(-1 == ret || 0 == ret)//读出错与文件结尾分支
			break;

		if(1 == Temp.m_Type){
			Temp.m_pObj = new CA;
			if( !(((CA*)(Temp.m_pObj))->DeSerialize(fd)) ){
				cout << "DeSerialize fail!" << endl;
				return false;
			}	
		}

		if(2 == Temp.m_Type){
			Temp.m_pObj = new CB;
			if( !(((CB*)(Temp.m_pObj))->DeSerialize(fd)) ){
				cout << "bDeSerialize fail!" << endl;
				return false;
			}	
		}

		Vec.push_back(Temp);
	}

	if(-1 == ::close(fd)){
		cout << "Close file fail!" << endl;
		return false;
	}

	return true;
}


int main(int argc , char* argv[]){
	CA a1(5),a2(10);
	CB b1(1.25),b2(2.5);
	StObj st1 = {.m_Type = 1,.m_pObj = &a1};
	StObj st2 = {.m_Type = 2,.m_pObj = &b1};
	StObj st3 = {.m_Type = 1,.m_pObj = &a2};
	StObj st4 = {.m_Type = 2,.m_pObj = &b2};

	vector<StObj> VecSrc;
	vector<StObj> Vecdst;

	VecSrc.push_back(st1);
	VecSrc.push_back(st2);
	VecSrc.push_back(st3);
	VecSrc.push_back(st4);

	CSerializer Ser;

	Ser.Serialize("a.txt",VecSrc);
	Ser.DeSerialize("a.txt",Vecdst);
	
	for(int i = 0; i < Vecdst.size() ; i++){
		if(1 == Vecdst[i].m_Type)
			cout <<((CA*)(Vecdst[i].m_pObj))->GetData() << endl;
		if(2 == Vecdst[i].m_Type)
			cout <<((CB*)(Vecdst[i].m_pObj))->GetData() << endl;
	}

	return 0;
}
