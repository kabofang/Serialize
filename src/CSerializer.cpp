#include "CSerializer.h"
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "CA.h"
#include "CB.h"
using namespace std;
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
