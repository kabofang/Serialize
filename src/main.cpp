#include <iostream>
#include <vector>
#include "CSerializer.h"
#include "CA.h"
#include "CB.h"
using namespace std;

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
