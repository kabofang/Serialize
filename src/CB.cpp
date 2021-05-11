#include "CB.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
using namespace std;

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