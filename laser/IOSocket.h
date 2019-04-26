#ifndef _IOSOCKET_H_
#define _IOSOCKET_H_
	/**
	* 该类继承自CIO类，主要功能是
    * 一个该类对象可以与一个传感器建立连接，并向该传感器发送指令。
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
	WSAEVENT m_recvEvent; //wsaEventSelect模型，与每个socket绑定的事件

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


