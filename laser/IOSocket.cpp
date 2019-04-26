#include "stdafx.h"
#include <string>
#include "IOSocket.h"
/*
当socket进行TCP连接的时候（也就是调用connect时），
一旦网络不通，或者是ip地址无效，就可能使整个线程阻塞。一般为30秒。
这是因为在阻塞模式下，调用任何winsock api函数，都会产生等待！
*/

IOSocket::IOSocket(CString _ip, int _port)
{
	m_ip = _ip;
	m_port = _port;
}

IOSocket::~IOSocket()
{
	//关闭，释放资源
	Close();
}

/*
	return @ 0  成功
	         -1 
*/
int IOSocket::Connect(){

	using namespace std;
	string strStl = CT2A(m_ip);
	const char *ipCh = strStl.c_str();

	/**
		socket函数 创建一个能够进行网络通信的套接字，创建出来的SOCKET是阻塞式的。
			AF_INET:表示IPV4网络协议；
			SOCK_STREAM:   TCP进行传输
			0：使用前两个参数指定的缺省协议！
	*/
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
		return -1;
	/**
		一般用函数inet_addr()把const char *cp型的IP地址转换成unsigned long型的整数值后再置给s_addr
	*/
	struct sockaddr_in serverAddr;//地址空间
	serverAddr.sin_addr.s_addr = inet_addr(ipCh);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(m_port);

	/*
		在send(),recv()过程中有时由于网络状况等原因，发收不能预期进行,而设置收发时限
	*/
	int nNetTimeout = 2000;//时限2秒
	setsockopt(m_socket,SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	setsockopt(m_socket,SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//返回一个手工重置的事件对象句柄（如果自动重置，可用CreateEvent）
	m_recvEvent = ::WSACreateEvent();
	/*
	WSAEventSelect事件模型，此步骤过后，m_socket由阻塞式变成非阻塞式。
	MSDN：The WSAAsyncSelect and WSAEventSelect functions automatically set a socket to nonblocking mode.
	*/
	::WSAEventSelect(m_socket, m_recvEvent, FD_READ | FD_CLOSE);

	/**
		对于流套接字（类型SOCK_STREAM），connect()函数用来初始化与参数name（套接字名字空间中的地址）指示的外部主机的活动连接.
			函数成功返回0，失败则返回-1 (SOCKET_ERROR）
	*/
	// 超时时间,非阻塞connect 
	struct timeval tm;
	tm.tv_sec = 3;//秒
	tm.tv_usec = 0;//微秒

	// 尝试去连接服务端 
	int sConnect = connect(m_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (sConnect == -1){
		fd_set set;
		FD_ZERO(&set);
		FD_SET(m_socket, &set);

		if (select(-1, NULL, &set, NULL, &tm) <= 0) //有错误(select错误或者超时)
			return -1;
		else
		{
			int error = -1;
			int optLen = sizeof(int);
			getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&error, &optLen);

			// 之所以下面的程序不写成三目运算符的形式， 是为了更直观， 便于注释  
			if (0 != error)
				return -1;
		}
	}
	SetIsConnect(true);
	return 0;
}

void IOSocket::Close(){
	if (GetIsConnect()){
		/*
			此函数关闭套接字s，并释放分配给该套接字的资源，以后对s的引用都将产生错误WSAENOTSOCK。
			如果s涉及一个打开的TCP连接，该连接被释放。
		*/
		closesocket(this->m_socket);
		this->m_socket = NULL;
		//调用WSACloseEvent函数，释放由事件句柄使用的系统资源
		WSACloseEvent(m_recvEvent);

		SetIsConnect(false);
	}
	//若本来就没有连接，则什么也不做
}

/**
return @ 非负 发送成功
		-1 发送失败
		-2 未建立连接
*/
int IOSocket::SendData(CString commend){

	using namespace std;
	string strStl = CT2A(commend);
	const char *commendChar = strStl.c_str();

	int copyNum;
	if (this->GetIsConnect())
	{
		/*
			send()函数用于在参数s指定的数据报或流套接字上向指定目的地发送输出数据。
				buf    指向存有发送数据的缓冲区的指针。
				len    缓冲区buf中数据长度
				flags  指定调用的方式，它可用来与套接字相关的选项一起影响函数的功能。就是说，sendto()函数的意义由套接字选项和flags参数共同决定。flags可取下述值：
		*/
		copyNum = send(this->m_socket, commendChar, strlen(commendChar), 0); //返回-1，表明发送失败	
		if (copyNum == SOCKET_ERROR)
		{
			return -1;
		}
		return copyNum;
	}
	else
	{
		return -2;
	}
}

void IOSocket::SetIp(CString _ip){
	m_ip = _ip;
}

CString IOSocket::GetIp(){
	return m_ip;
}


void IOSocket::SetPort(int _port){
	m_port = _port;
}

int IOSocket::GetPort(){
	return m_port;
}

 
SOCKET IOSocket::GetSocket(){
	return m_socket;
}

WSAEVENT IOSocket::GetRecvEvent(){
	return m_recvEvent;
}

void IOSocket::SetRecvEvent(WSAEVENT eve){
	m_recvEvent = eve;
}


