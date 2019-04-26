#ifndef _CIO_H_
#define _CIO_H_
	/**
	* 该类的主要功能是搭建一个与传感器连接的框架，具体如何
    * 与传感器建立连接由对应的子类具体实现。
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