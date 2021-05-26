#ifndef _CB_H_
#define _CB_H_
#include "CSerializable.h"
#include "st.h"
class CB : public CSerializable{
private:
        int m_i1;
        int m_i2;
        double m_d;
        TestData m_st;
public:
        explicit CB(int x=0,int y=0,double z=0,int v=0,double w=0);
    
        void DispData(void) const;
    
        int Serialize(char *pbuf) const;
        pCS DeSerialize(char *pbuf,int *plen);

        bool GetType(int& type);
};

#endif