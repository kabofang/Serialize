#include "CA.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
using namespace std;
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