#include "stdafx.h"
#include "Sensor.h"
#include "IOSocket.h"
#include "SyncCondition.h"
#include "LaserDef.h"
#include "MessageProcess.h"

Sensor::Sensor(){
	m_brand = KEYENCE;
	m_id = -1;
	
	m_request = _T("");
	m_response = _T("");
	m_ip = _T("");;
	m_port = 0;
	m_IoSocket = nullptr;
	m_pRecvCond = new SyncCondition();
	m_isRequesting = false;
}

Sensor::~Sensor()
{   
	if (m_pRecvCond != nullptr){
		delete m_pRecvCond;
		m_pRecvCond = nullptr;
	}
}

void Sensor::SetBrand(SensorBrand _brand){
	m_brand = _brand;
}

SensorBrand Sensor::GetBrand(){
	return m_brand;
}

void Sensor::SetId(int id){
	m_id = id;
}

int Sensor::GetId(){
	return m_id;
}

void Sensor::SetResponse(CString res){
	m_response = res;
}

CString Sensor::GetResponse(){
	return m_response;
}


void Sensor::SetRequest(CString req){
	m_request = req;
}

CString Sensor::GetRequest(){
	return m_request;
}

void Sensor::SetIsRequesting(bool _bool){
	m_isRequesting = _bool;
}

bool Sensor::GetIsRequesting(){
	return m_isRequesting;
}

void Sensor::SetIp(CString _ip){
	m_ip = _ip;
}

CString Sensor::GetIp(){
	return m_ip;
}


void Sensor::SetPort(int _port){
	m_port = _port;
}

int Sensor::GetPort(){
	return m_port;
}

void Sensor::SetIoSocket(IOSocket* const socket){
		m_IoSocket = socket;
}

IOSocket* Sensor::GetIoSocket(){
	return m_IoSocket;
}

void Sensor::InitIoSocket(IOSocket* const socket){
	if (socket == nullptr)
		m_IoSocket = new IOSocket(m_ip, m_port);
	else
		m_IoSocket = socket;
}

SyncCondition* Sensor::GetRecvCondition(){
	return m_pRecvCond;
}

/*
return @ 0  ����
-1
�÷���������ʵ���ӵ�ʵ��ϸ��
�����ǲ��� ��Sensor �������ӣ�ֻҪû�н������ӣ��ͽ�������
��������Sensor����ֽ������ӣ������Ѿ����������ˣ���ʲô����������
ֱ�ӷ��ص�����ʵ����IOSocket������״̬����m_IoSocket->GetIsConnect();
*/
int Sensor::Connect(){
	int code = 0;
	if (!m_IoSocket->GetIsConnect()){
		code = m_IoSocket->Connect();
	}
	return code;
}

void Sensor::BreakConnect(){
	/*
	�÷���������ʵ���ӵ�ʵ��ϸ��
	ֻҪ����Ͽ������ˣ�ͬ��һ��socket���ӵ�sensor�����
	Sensor::GetIsConnect()���ص�����ʵ����IOSocket������״̬��
	��m_IoSocket->GetIsConnect();�����൱��ȫ��֪ͨ�˹���һ��IOSocket������
	sensor������״̬��Ρ�
	*/
	if (m_IoSocket->GetIsConnect()){
		m_IoSocket->Close();
	}
}

bool Sensor::GetIsConnect(){
	return m_IoSocket->GetIsConnect();
}

void Sensor::SetIsConnect(bool _connect){
	m_IoSocket->SetIsConnect(_connect);
}

SOCKET Sensor::GetSocket(){
	return m_IoSocket->GetSocket();
}

WSAEVENT Sensor::GetWsaEvent(){
	return m_IoSocket->GetRecvEvent();
}

void Sensor::ReceivedSignal(){
	m_pRecvCond->Signal();
}

CString Sensor::RequestSensor()
{
	//������Ϊ��
	SetIsRequesting(true);
	CString recvData = _T("");

	//ȷ��������Ϣ֮ǰΪδ����  ?��Ҫ�������ڣ� ���ⲻ�Ǳ�������ʹͬ���źŴ��������
	m_pRecvCond->ResetSignal();
	m_response = _T("");//ȡ������֮ǰ�ÿ�

	int code = m_IoSocket->SendData(m_request);
	if (code < 0)
	{
		//������ɣ�������Ϊ��
		if (code == -1)
		{
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("�򴫸�����������ʧ��"),m_id);
		}
		else if (code == -2)
		{
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("��λ���봫����δ��������"), m_id);
		}
		SetIsRequesting(false);
		recvData = SENSOR_REQ_ERROR;
		return recvData;
	}

	//wait �����̣߳�����Ӱ��Ч��
	DWORD dw = m_pRecvCond->Wait(1000);
	if (dw != WAIT_OBJECT_0)
	{
		SetIsRequesting(false);
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("���󴫸������ݵȴ���ʱ"), m_id);
		recvData = SENSOR_REQ_ERROR;
		return recvData;
	}
	//�յ��������ݣ���������ִ��
	recvData = m_response;
	//������ɣ�������Ϊ��
	m_isRequesting = false;

	return recvData;
}