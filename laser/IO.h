#ifndef _CIO_H_
#define _CIO_H_
	/**
	* �������Ҫ�����Ǵһ���봫�������ӵĿ�ܣ��������
    * �봫�������������ɶ�Ӧ���������ʵ�֡�
	* @author zjs
	* @Time 2017
	*/
class CIO
{
public:
	CIO();
	~CIO();

private:
	bool m_bConnect;

public:
	virtual int Connect() = 0;
	virtual void Close() = 0;

	bool GetIsConnect();
	void SetIsConnect(bool bl);
};

#endif