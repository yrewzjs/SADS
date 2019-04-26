#ifndef _IOSOCKET_H_
#define _IOSOCKET_H_
	/**
	* ����̳���CIO�࣬��Ҫ������
    * һ��������������һ���������������ӣ�����ô���������ָ�
	* @author zjs
	* @Time 2017
	*/
#include "IO.h"
class IOSocket:public CIO
{
public:
	IOSocket(CString _ip,int _port);
	~IOSocket();

private:
	SOCKET m_socket;
	WSAEVENT m_recvEvent; //wsaEventSelectģ�ͣ���ÿ��socket�󶨵��¼�

	CString m_ip;
	int m_port;

public:

	int Connect();

	void Close();

	int SendData(CString commend);

	void SetIp(CString _ip);
	CString GetIp();

	void SetPort(int _port);
	int GetPort();

	SOCKET GetSocket();

	WSAEVENT GetRecvEvent();

	void SetRecvEvent(WSAEVENT eve);

};


#endif


