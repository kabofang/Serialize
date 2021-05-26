#include "CStoreFileStrategy.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

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

