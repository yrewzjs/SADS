#include "stdafx.h"
#include "Group.h"
#include "IpGroup.h"

Group::Group()
{
	m_SensorMap = new map<int, Sensor*>();
	m_IpGroupMap = new map<int, IpGroup*>();
}

Group::~Group()
{
	if (m_SensorMap != nullptr){
		//清空
		m_SensorMap->clear();
		delete m_SensorMap;
		m_SensorMap = nullptr;
	}

	if (m_IpGroupMap != nullptr){
		map<int, IpGroup*>::iterator iter = m_IpGroupMap->begin();
		while (iter != m_IpGroupMap->end())
		{
			//回收IpGroup内存
			delete (iter->second);
			iter++;
		}
		//清空
		m_IpGroupMap->clear();
		delete m_IpGroupMap;
		m_IpGroupMap = nullptr;
	}
}

void Group::SetId(int id){
	m_id = id;
}

int Group::GetId(){
	return m_id;
}

void Group::SetSensorNum(int _num){
	m_sensorNum = _num;
}

int Group::GetSensorNum(){
	return m_SensorMap->size();
}

int Group::GetIpGroupNum(){
	return m_IpGroupMap->size();
}

void Group::pushElement(int id, Sensor* const pSen){
	m_SensorMap->insert(map<int, Sensor*>::value_type(id, pSen));
}

void Group::pushIpGroup(int id, IpGroup* const pIpGroup){
	m_IpGroupMap->insert(map<int, IpGroup*>::value_type(id, pIpGroup));
}


Sensor* Group::GetSensor(int id){
	Sensor* pSen = nullptr;
	map<int, Sensor* >::iterator l_it;
	l_it = m_SensorMap->find(id);
	//检查是否找到
	if (l_it != m_SensorMap->end())
		pSen = l_it->second;

	return pSen;
}

map<int, Sensor*>* Group::GetSensorMap(){
	return m_SensorMap;
}

map<int, IpGroup*>* Group::GetIpGroupMap()
{
	return m_IpGroupMap;
}

IpGroup* Group::GetIpGroup(int ipgroupid){
	IpGroup* pIpGroup = nullptr;
	map<int, IpGroup* >::iterator l_it;
	l_it = m_IpGroupMap->find(ipgroupid);
	//检查是否找到
	if (l_it != m_IpGroupMap->end())
		pIpGroup = l_it->second;

	return pIpGroup;
}


