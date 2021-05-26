#ifndef _CSERIALIZABLE_H_
#define _CSERIALIZABLE_H_
class CSerializable{
public:
        virtual int Serialize(char *pbuf) const = 0;
        virtual CSerializable* DeSerialize(char *pbuf,int *plen) = 0;
        virtual bool GetType(int& type) = 0;
};
typedef CSerializable* pCS;

#endif