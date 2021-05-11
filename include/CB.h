#ifndef _CB_H_
#define _CB_H_
class CB{
private:
	double m_d;
public:
	CB() { m_d=0; }
	explicit CB(double x) : m_d(x) {}

	void SetData(double x) {m_d = x;}
	double GetData(void) { return m_d; }

	bool Serialize(int fd) const;
	bool DeSerialize(int fd);
};
#endif