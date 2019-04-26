#include "stdafx.h"

#include "Calculate.h"
#include "SensorManager.h"
#include "Sensor.h"

#include "ConfigFileUtill.h"
#include "Group.h"
#include "IpGroup.h"
#include "MessageProcess.h"
#include "LaserDef.h"

SensorManager::SensorManager()
{
	//初始化map
	m_SensorMap = new map<int, Sensor*>();
	m_GroupMap = new map<int, Group*>();
}

SensorManager::~SensorManager()
{
	//释放m_SensorVector指针元素指向的空间

	if (m_SensorMap != nullptr){
		//迭代器,遍历删除需要注意
		map<int, Sensor*>::iterator iter = m_SensorMap->begin();
		while(iter != m_SensorMap->end())
		{
			//回收Sensor内存
			delete (iter->second);
			iter++;
		}
		//清空
		m_SensorMap->clear();
		delete m_SensorMap;
		m_SensorMap = nullptr;
	}

	if (m_GroupMap != nullptr){
		map<int, Group*>::iterator iter = m_GroupMap->begin();
		while (iter != m_GroupMap->end())
		{
			//回收Group内存
			delete (iter->second);
			iter++;
		}
		//清空
		m_GroupMap->clear();
		delete m_GroupMap;
		m_GroupMap = nullptr;
	}
}

bool SensorManager::InitSensorManager(){
	//解析配置文件
	bool isParse = ConfigFileUtill::LoadLaserConfigFile(this);
	if (isParse)
	{
		//与Nport建立连接
		CString failedId = _T("");
		bool isLink = ConnectLinks(failedId);
		if (isLink)
		{
			return true;
		}
		else
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("SOCKET ") + failedId + _T(" Connect Failed，请检查计算机与服务器之间线路、配置文件IP地址"));
			return false;
		}
	}
	return false;
}

int SensorManager::GetSenNum()
{
	return m_SensorMap->size();
}

int SensorManager::GetGroupNum()
{
	return m_GroupMap->size();
}

Sensor* SensorManager::GetSensor(int id)
{
	Sensor* pSen = nullptr;
	map<int, Sensor* >::iterator l_it;
	l_it = m_SensorMap->find(id);
	//检查是否找到
	if (l_it != m_SensorMap->end())
		pSen = l_it->second;

	return pSen;
}

map<int, Sensor*>* SensorManager::GetSensorMap()
{
	return m_SensorMap;
}

Sensor* SensorManager::GetCurrentSensor(SOCKET _sock)
{
	Sensor* pSen = nullptr;
	map<int, Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSen = iter->second;
		if (_sock == pSen->GetSocket())
			if (pSen->GetIsRequesting())
				return pSen;
		
		iter++;
	}
	return nullptr;
}

Sensor* SensorManager::GetMainSensor(CString ip, int port)
{
	Sensor* pSen = nullptr;
	map<int, Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSen = iter->second;
		if (pSen->GetBrand() == SICK)
		{
			iter++;
			continue;
		}

		if (pSen != nullptr)
		{
			if (ip == pSen->GetIp() && port == pSen->GetPort())
			{
				//SensorKeyence* pSenKey = (SensorKeyence*)pSen;
				if (!pSen->GetIsVice())
					return pSen;
			}
		}
		iter++;
	}
	return nullptr;
}

bool SensorManager::GetAllLaserState(CString &closeId)
{
	bool returnFlag = true;
	bool flag;
	Sensor* pSen = nullptr;
	CString idStr;
	int id = -1;

	map<int, Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSen = iter->second;
		flag = pSen->GetLaserState();
		if (!flag)
		{
			returnFlag = false;
			id = pSen->GetId();
			idStr.Format(_T("%2d"), id);
			closeId += idStr;
		}
		iter++;
	}
	return returnFlag;
}

bool SensorManager::ConnectLinks(CString &failedId)
{
	Sensor* pSen = nullptr;
	CString idStr;
	bool returnFlag = true;

	map<int,Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSen = iter->second;
		if (pSen->GetBrand() == KEYENCE)
		{
			if (pSen->GetIsVice())
			{
				iter++;
				continue;
			}
		}

		if (pSen->Connect()!= 0)
		{
			idStr.Format(_T("%4d,"), pSen->GetPort());
			failedId += idStr;
			returnFlag = false;
		}
		else
		{
			LOG(INFO) << MessageProcess::ProcessMessage(_T("SOCKET Connect Succeed"), pSen->GetPort());
		}

		iter++; // 迭代器后移
	}
	return returnFlag;
}

bool SensorManager::CheckConnectState(CString &failedId)
{
	Sensor* pSensor = nullptr;
	CString idStr, validData;
	bool returnFlag = true;
	map<int, Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSensor = iter->second;
		if (pSensor->GetIsVice())
		{
			iter++;
			continue;
		}
		//主传感器请求数据
		CString validData;
		pSensor->AcquireGroupResponse(validData);
		if (validData != SENSOR_GROUP_REQ_ERROR)
		{
			iter++;
			continue;
		}
		else
		{
			returnFlag = false;
			idStr.Format(_T("%2d"), pSensor->GetId());
			failedId += idStr;
		}
		//迭代器更新
		iter++;
	}
	return returnFlag;
}

Group* SensorManager::GetGroup(int groupid)
{
	Group* pGroup = nullptr;
	map<int, Group* >::iterator l_it;
	l_it = m_GroupMap->find(groupid);
	//检查是否找到
	if (l_it != m_GroupMap->end())
		pGroup = l_it->second;

	return pGroup;

}

void SensorManager::pushSenElement(int id, Sensor* const pSen)
{
	m_SensorMap->insert(map<int, Sensor*>::value_type(id, pSen));
}

void SensorManager::pushGrpElement(int id, Group* const pGroup){
	m_GroupMap->insert(map<int, Group*>::value_type(id, pGroup));
}

bool SensorManager::JudgeUnitId(int port, int suid)
{
	IpGroup* pIpGroup = nullptr;
	Group* pGroup = nullptr;

	map<int, Group* >::iterator g_it = m_GroupMap->begin();
	while (g_it != m_GroupMap->end())
	{
		pGroup = g_it->second;
		
		map<int, IpGroup* >::iterator l_it = pGroup->GetIpGroupMap()->begin();
		while (l_it != pGroup->GetIpGroupMap()->end())
		{
			pIpGroup = l_it->second;
			if (port == pIpGroup->GetPort() && suid == pIpGroup->GetIpSenNum())
				return true;
			l_it++;
		}
		g_it++;
	}
	return false;
}

bool SensorManager::CloseAllLaser(CString &failedId)
{
	bool returnFlag = true;
	Sensor *pSensor = nullptr;
	bool isSuccess = false;
	CString idStr;

	map<int, Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSensor = iter->second;
		//关闭激光
		if (!pSensor->GetLaserState())
		{
			iter++;//迭代器后移一位
			continue;
		}

		isSuccess = pSensor->CloseLaser();
		if (!isSuccess){
			returnFlag = false;
			idStr.Format(_T("%02d"), pSensor->GetId());
			failedId += idStr;
		}
		iter++;//迭代器后移一位
	}
	return returnFlag;
}

bool SensorManager::TriggerAllLaser(CString &failedId)
{
	bool returnFlag = true;
	Sensor *pSensor = nullptr;
	bool isSuccess;
	CString idStr;

	map<int, Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSensor = iter->second;

		if (pSensor->GetLaserState())
		{
			iter++;//迭代器后移一位
			continue;
		}
		//开启激光
		isSuccess = pSensor->TriggerLaser();
		if (!isSuccess){
			returnFlag = false;
			idStr.Format(_T("%02d"), pSensor->GetId());
			failedId += idStr;
		}
		iter++;//迭代器后移一位
	}

	return returnFlag;
}

bool SensorManager::ContainsSensor(int id)
{
	/**
		返回map中键k出现的次数，这里只返回0(不存在)和1(存在)
	*/
	if (m_SensorMap->count(id) != 0)
		return true;
	else
		return false;
}
