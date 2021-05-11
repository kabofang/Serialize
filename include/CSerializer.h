#ifndef _CSERIALIZE_H_
#define _CSERIALIZE_H_
#include <vector>
#include "StObj.h"
using namespace std;
class CSerializer{
public://问老师static与之区别
	bool Serialize(const char* pFilePath , const vector<StObj> &Vec) const;
	bool DeSerialize(const char* pFilePath , vector<StObj> &Vec);
};
#endif