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
return @ 0  连接
-1
该方法隐藏真实连接的实现细节
不管是不是 主Sensor 建立连接，只要没有建立连接，就建立连接
就算是主Sensor后出现建立连接，发现已经建立连接了，则什么都不用做，
直接返回的是真实连接IOSocket的连接状态，即m_IoSocket->GetIsConnect();
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
	该方法隐藏真实连接的实现细节
	只要这里断开连接了；同用一个socket连接的sensor对象的
	Sensor::GetIsConnect()返回的是真实连接IOSocket的连接状态，
	即m_IoSocket->GetIsConnect();所以相当于全局通知了共用一个IOSocket的其他
	sensor的连接状态如何。
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
	//将其置为真
	SetIsRequesting(true);
	CString recvData = _T("");

	//确保发送信息之前为未发信  ?必要性体现在？ 避免不是本次请求使同步信号触发的情况
	m_pRecvCond->ResetSignal();
	m_response = _T("");//取得数据之前置空

	int code = m_IoSocket->SendData(m_request);
	if (code < 0)
	{
		//请求完成，将其置为假
		if (code == -1)
		{
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("向传感器发送请求失败"),m_id);
		}
		else if (code == -2)
		{
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("上位机与传感器未建立连接"), m_id);
		}
		SetIsRequesting(false);
		recvData = SENSOR_REQ_ERROR;
		return recvData;
	}

	//wait 阻塞线程，严重影响效率
	DWORD dw = m_pRecvCond->Wait(1000);
	if (dw != WAIT_OBJECT_0)
	{
		SetIsRequesting(false);
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("请求传感器数据等待超时"), m_id);
		recvData = SENSOR_REQ_ERROR;
		return recvData;
	}
	//收到请求数据，继续向下执行
	recvData = m_response;
	//请求完成，将其置为假
	m_isRequesting = false;

	return recvData;
}