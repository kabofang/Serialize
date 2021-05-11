#ifndef _CA_H_
#define _CA_H_
class CA{
private:
	int m_i;
public:
	CA() { m_i=0; }
	explicit CA(int x) : m_i(x) {}

	void SetData(int x) {m_i = x;}
	int GetData(void) { return m_i; }

	bool Serialize(int fd) const;
	bool DeSerialize(int fd);
};
#endif