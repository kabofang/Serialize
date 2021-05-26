#ifndef _CC_H_
#define _CC_H_
#include "CSerializable.h"
#include "st.h"
class CC : public CSerializable{
private:
        int m_i;
        double m_d1;
        double m_d2;
        TestData m_st;
public:
        explicit CC(int x=0,double y=0,double z=0,int v=0,double w=0);
        void DispData(void) const;

        int Serialize(char *pbuf) const;
        pCS DeSerialize(char *pbuf,int *plen);

        bool GetType(int& type);
};

#endif