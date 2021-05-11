#include <iostream>
#include <unistd.h>
#include <fcntl.h>
using namespace std;
class A{
private:
	int m_i;
public:
	A() { m_i=0; }
	explicit A(int x) : m_i(x) {}

	void SetData(int x) {m_i = x;}
	int GetData(void) { return m_i; }

	bool Serialize(const char* pFilePath) const;
	bool DeSerialize(const char* pFilePath);
};

bool A::Serialize(const char* pFilePath)const{
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_CREAT|O_WRONLY|O_TRUNC,00600))){
		cout<<"File open fail!"<<endl;
		return false;
	}
	if(-1 == ::write(fd,&m_i,sizeof(m_i))){
		cout<<"Write file fail!"<<endl;
		return false;
	}
	if(-1 == ::close(fd)){
		cout<<"Close file fail!"<<endl;
		return false;
	}
	return true;
}
bool A::DeSerialize(const char* pFilePath){
	int fd;
	if(-1 == (fd = ::open(pFilePath,O_RDONLY))){
		cout<<"File open fail!"<<endl;
		return false;
	}
	if(-1 == ::read(fd,&m_i,sizeof(m_i))){
		cout<<"Read file fail!"<<endl;
		return false;
	}
	if(-1 == ::close(fd)){
		cout<<"Close file fail!"<<endl;
		return false;
	}
	return true;
}

int main(int argc , char* argv[]){
	A a1(5);
	A a2;

	a1.Serialize("a.txt");
	a2.DeSerialize("a.txt");

	cout<<a2.GetData()<<endl;

	return 0;
}
