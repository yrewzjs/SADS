#include "stdafx.h"
#include <string>
#include "IOSocket.h"
/*
��socket����TCP���ӵ�ʱ��Ҳ���ǵ���connectʱ����
һ�����粻ͨ��������ip��ַ��Ч���Ϳ���ʹ�����߳�������һ��Ϊ30�롣
������Ϊ������ģʽ�£������κ�winsock api��������������ȴ���
*/

IOSocket::IOSocket(CString _ip, int _port)
{
	m_ip = _ip;
	m_port = _port;
}

IOSocket::~IOSocket()
{
	//�رգ��ͷ���Դ
	Close();
}

/*
	return @ 0  �ɹ�
	         -1 
*/
int IOSocket::Connect(){

	using namespace std;
	string strStl = CT2A(m_ip);
	const char *ipCh = strStl.c_str();

	/**
		socket���� ����һ���ܹ���������ͨ�ŵ��׽��֣�����������SOCKET������ʽ�ġ�
			AF_INET:��ʾIPV4����Э�飻
			SOCK_STREAM:   TCP���д���
			0��ʹ��ǰ��������ָ����ȱʡЭ�飡
	*/
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
		return -1;
	/**
		һ���ú���inet_addr()��const char *cp�͵�IP��ַת����unsigned long�͵�����ֵ�����ø�s_addr
	*/
	struct sockaddr_in serverAddr;//��ַ�ռ�
	serverAddr.sin_addr.s_addr = inet_addr(ipCh);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(m_port);

	/*
		��send(),recv()��������ʱ��������״����ԭ�򣬷��ղ���Ԥ�ڽ���,�������շ�ʱ��
	*/
	int nNetTimeout = 2000;//ʱ��2��
	setsockopt(m_socket,SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	setsockopt(m_socket,SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//����һ���ֹ����õ��¼�������������Զ����ã�����CreateEvent��
	m_recvEvent = ::WSACreateEvent();
	/*
	WSAEventSelect�¼�ģ�ͣ��˲������m_socket������ʽ��ɷ�����ʽ��
	MSDN��The WSAAsyncSelect and WSAEventSelect functions automatically set a socket to nonblocking mode.
	*/
	::WSAEventSelect(m_socket, m_recvEvent, FD_READ | FD_CLOSE);

	/**
		�������׽��֣�����SOCK_STREAM����connect()����������ʼ�������name���׽������ֿռ��еĵ�ַ��ָʾ���ⲿ�����Ļ����.
			�����ɹ�����0��ʧ���򷵻�-1 (SOCKET_ERROR��
	*/
	// ��ʱʱ��,������connect 
	struct timeval tm;
	tm.tv_sec = 3;//��
	tm.tv_usec = 0;//΢��

	// ����ȥ���ӷ���� 
	int sConnect = connect(m_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (sConnect == -1){
		fd_set set;
		FD_ZERO(&set);
		FD_SET(m_socket, &set);

		if (select(-1, NULL, &set, NULL, &tm) <= 0) //�д���(select������߳�ʱ)
			return -1;
		else
		{
			int error = -1;
			int optLen = sizeof(int);
			getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&error, &optLen);

			// ֮��������ĳ���д����Ŀ���������ʽ�� ��Ϊ�˸�ֱ�ۣ� ����ע��  
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
			�˺����ر��׽���s�����ͷŷ�������׽��ֵ���Դ���Ժ��s�����ö�����������WSAENOTSOCK��
			���s�漰һ���򿪵�TCP���ӣ������ӱ��ͷš�
		*/
		closesocket(this->m_socket);
		this->m_socket = NULL;
		//����WSACloseEvent�������ͷ����¼����ʹ�õ�ϵͳ��Դ
		WSACloseEvent(m_recvEvent);

		SetIsConnect(false);
	}
	//��������û�����ӣ���ʲôҲ����
}

/**
return @ �Ǹ� ���ͳɹ�
		-1 ����ʧ��
		-2 δ��������
*/
int IOSocket::SendData(CString commend){

	using namespace std;
	string strStl = CT2A(commend);
	const char *commendChar = strStl.c_str();

	int copyNum;
	if (this->GetIsConnect())
	{
		/*
			send()���������ڲ���sָ�������ݱ������׽�������ָ��Ŀ�ĵط���������ݡ�
				buf    ָ����з������ݵĻ�������ָ�롣
				len    ������buf�����ݳ���
				flags  ָ�����õķ�ʽ�������������׽�����ص�ѡ��һ��Ӱ�캯���Ĺ��ܡ�����˵��sendto()�������������׽���ѡ���flags������ͬ������flags��ȡ����ֵ��
		*/
		copyNum = send(this->m_socket, commendChar, strlen(commendChar), 0); //����-1����������ʧ��	
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


