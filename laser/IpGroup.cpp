#include "stdafx.h"
#include "IpGroup.h"
#include "Sensor.h"
#include "MessageProcess.h"
#include "LaserDef.h"

IpGroup::IpGroup()
{
	m_IpSensorMap = new map<int, Sensor*>();
}


IpGroup::~IpGroup()
{
	if (m_IpSensorMap != nullptr){
		//清空
		m_IpSensorMap->clear();
		delete m_IpSensorMap;
		m_IpSensorMap = nullptr;
	}
}

void IpGroup::pushElement(int uid, Sensor* const pSen){
	m_IpSensorMap->insert(map<int, Sensor*>::value_type(uid, pSen));
}

Sensor* IpGroup::GetSensor(int uid){
	Sensor* pSen = nullptr;
	map<int, Sensor* >::iterator l_it;
	l_it = m_IpSensorMap->find(uid);
	//检查是否找到
	if (l_it != m_IpSensorMap->end())
		pSen = l_it->second;
	return pSen;
}

Sensor* IpGroup::GetSensor(){
	Sensor* pSen = nullptr;
	map<int, Sensor* >::iterator l_it;
	l_it = m_IpSensorMap->begin();
	//检查是否找到
	if (l_it != m_IpSensorMap->end())
		pSen = l_it->second;
	return pSen;
}

map<int, Sensor*>* IpGroup::GetIpSensorMap(){
	return m_IpSensorMap;
}

int IpGroup::GetIpSenNum(){
	return m_IpSensorMap->size();
}

void IpGroup::SetIp(CString _ip){
	m_ip = _ip;
}

CString IpGroup::GetIp(){
	return m_ip;
}


void IpGroup::SetPort(int _port){
	m_port = _port;
}

int IpGroup::GetPort(){
	return m_port;
}

void IpGroup::SetId(int id){
	m_id = id;
}

int IpGroup::GetId(){
	return m_id;
}

void IpGroup::RequestIpGroupSensor(CString & validData)
{
	Sensor* pSen = GetSensor();
	if (pSen != nullptr)
	{
		pSen->AcquireGroupResponse(validData);
	} 
	else
	{
		validData = SENSOR_GROUP_REQ_ERROR;
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("GetSensor返回空指针"));
	}
}