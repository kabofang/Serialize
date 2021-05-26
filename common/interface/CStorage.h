#ifndef _CSERIALIZABLE_H__
#define _CSERIALIZABLE_H__

#include "CSerializable.h"
#include "CStoreStrategy.h"
#include <vector>

using namespace std;

class CStorage{
public:
    CStoreStrategy *m_Strategy;
public:
    virtual bool Serialize(const vector<pCS> &Vec) const = 0;
    virtual bool DeSerialize(vector<pCS> &Vec) = 0;
};

#endif