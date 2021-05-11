#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
using namespace std;
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

class CSerializer{
public://问老师static与之区别
	bool Serialize(const char* pFilePath , const vector<CA> &Vec) const;
	bool DeSerialize(const char* pFilePath , vector<CA> &Vec);
};

bool CSerializer::Serialize(const char* pFilePath , const vector<CA> &Vec) const{
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_CREAT|O_TRUNC|O_WRONLY,00600))){
		cout << "Open file fail!" << endl;
		return false;
	}

	for(int i = 0 ; i < Vec.size() ; i++)
		if(!Vec[i].Serialize(fd)){
			cout << "Serialize fail!" << endl;
			return false;
		}

	if(-1 == ::close(fd)){
		cout << "Close file fail!" << endl;
		return false;
	}
	return true;
}
bool CSerializer::DeSerialize(const char* pFilePath , vector<CA> &Vec){	
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_RDONLY))){
		cout << "Open file fail!" << endl;
		return false;
	}

	for(;;){
		CA Temp;
		if(!Temp.DeSerialize(fd))//失败与文件尾的分支？
			break;
		Vec.push_back(Temp);
	}

	if(-1 == ::close(fd)){
		cout << "Close file fail!" << endl;
		return false;
	}
	return true;
}


int main(int argc , char* argv[]){
	CA a[10];
	vector<CA> VecA;
	vector<CA> Vecdst;

	for(int i = 0 ; i < 10 ; i++){
		a[i].SetData(2*i);
		VecA.push_back(a[i]);
	}

	CSerializer Ser;

	Ser.Serialize("a.txt",VecA);
	Ser.DeSerialize("a.txt",Vecdst);

	for(int i = 0 ; i < Vecdst.size() ; i++)
		cout<<Vecdst[i].GetData()<<endl;

	return 0;
}
