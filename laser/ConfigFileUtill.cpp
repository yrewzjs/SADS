#include "stdafx.h"
#include <string>
//#include <regex>
#include <set>

#include "tinystr.h"   
#include "tinyxml.h"

#include "ConfigFileUtill.h"
#include "MessageProcess.h"
#include "LaserDef.h"

#include "Group.h"
#include "IpGroup.h"
#include "SensorData.h"
#include "SensorManager.h"
#include "Sensor.h"
#include "SensorFactory.h"

#include "Calculate.h"

bool ConfigFileUtill::LoadLaserConfigFile(SensorManager* pSenManager)
{
	// 读入XML文件
	CString filePath = MessageProcess::GetFilePath(_T("ini\\LaserModuleIni.xml")); //文件名
	using namespace std;
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	if (!doc.LoadFile(path))
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("读取配置文件失败，可能原因：配置文件格式错误、路径错误"));
		return false;
	}

	Sensor *pSen = nullptr;
	Sensor *pMainSen = nullptr;
	SensorFactory* factory = SensorFactory::GetSensorFactory();
	Group* pGroup = nullptr;
	IpGroup* pIpGroup = nullptr;
	set<int> portSet; //port 检查使用
	CString brand;
	int port;
	int errorCode;
	CString ip;
	int id = 0; //传感器ID 从0递增。
	int groupId = 0;//Group ID 从0递增。
	/*
	for 1：遍历拿到每一个<group>标签
	for 2：遍历拿到每一个<ipgroup>标签
	for 3：遍历拿到每一个<sensor>标签
	*/
	//开始解析
	TiXmlElement* root = doc.RootElement();//根标签
	if (root == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件root标签异常"));
		return false;
	}

	TiXmlElement* gElem = root->FirstChildElement();
	if (gElem == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件root标签异常"));
		return false;
	}

	TiXmlElement* ipElem = NULL;
	TiXmlElement* sElem = NULL;
	TiXmlElement* calElem = NULL;
	TiXmlElement* gcheightElem = NULL;
	TiXmlAttribute *attr = NULL;

	/***********    进入<group>标签  *********************/
	int loadedGroupId = -1;
	for (gElem; gElem != 0; gElem = gElem->NextSiblingElement())
	{
		pGroup = new Group();
		//设置Group ID
		errorCode = gElem->QueryIntAttribute("id", &groupId);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<group>标签id属性类型异常"));
			return false;
		}
		else if (groupId < 0 || groupId > 1)
		{   
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<ipgroup>标签id属性数值错"));
			return false;
		}
		else if (groupId == loadedGroupId)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<ipgroup>标签id属性重复"));
			return false;
		}
		loadedGroupId = groupId;//已载入Group id
		pGroup->SetId(groupId);

		if (gElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<group>标签异常"));
			return false;
		}
		/***********    进入<ipGroup>标签  *********************/
		int ipGroupId = 0;
		ipElem = gElem->FirstChildElement();
		for (ipElem; ipElem != 0; ipElem = ipElem->NextSiblingElement())
		{
			if (ipElem == NULL)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<ipgroup>标签异常"));
				return false;
			}
			//生成一个IpGroup对象。
			pIpGroup = new IpGroup();
			pIpGroup->SetId(ipGroupId);

			//取出ip
			attr = ipElem->LastAttribute();
			ip = attr->Value();
			string str = CT2CA(ip);
			//设置ipGroup Ip
			pIpGroup->SetIp(ip);
			//分配该ipgroup的起始suId。
			int suId = 0;
			errorCode = ipElem->QueryIntAttribute("suid", &suId);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件suid类型异常"), ipGroupId);
				return false;
			}
			else if (suId<0 || suId>8)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件suid数值异常"), port);
				return false;
			}

			//获取port
			errorCode = ipElem->QueryIntAttribute("port", &port);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件端口类型异常"), port);
				return false;
			}
			else if (port<1024)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件端口数值异常"), port);
				return false;
			}

			//检查suid
			if (suId != 0 || portSet.count(port) != 0)
			{
				if (!pSenManager->JudgeUnitId(port, suId))
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<ipgroup>标签suId数值异常"), port);
					return false;
				}
			}

			//设置ipGroup port
			pIpGroup->SetPort(port);
			portSet.insert(port);

			//拿到第一个<sensor>标签，开始遍历
			sElem = ipElem->FirstChildElement();
			int unitId = suId;
			/***********    进入<sensor>标签  *********************/
			for (sElem; sElem != 0; sElem = sElem->NextSiblingElement())
			{
				if (sElem == NULL)
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<sensor>标签异常"));
					return false;
				}

				//设置品牌
				brand = sElem->FirstChildElement("brand")->GetText();
				if (brand == _T("KEYENCE"))
				{
					pSen = factory->GetSensor(KEYENCE);//初始化对象
					pSen->SetUnitId(unitId);//并设置unitId
					//并设置IsVice
					if (unitId == 0)
						pSen->SetIsVice(false);
					else
						pSen->SetIsVice(true);
					//并设置Brand
					pSen->SetBrand(KEYENCE);
				}
				else if (brand == _T("SICK"))
				{
					pSen = factory->GetSensor(SICK);
					pSen->SetBrand(SICK);
				}
				else{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<brand>标签异常"), port);

					return false;
				}

				//设置ip
				pSen->SetIp(ip);
				//设置port
				pSen->SetPort(port);
				//设置id
				errorCode = sElem->QueryIntAttribute("id", &id);
				if (errorCode == TIXML_WRONG_TYPE)
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<sensor>标签id属性类型异常"), port);
					return false;
				}
				else if (pSenManager->ContainsSensor(id))
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("<sensor>标签id重复异常"), id);
					return false;
				}
				else if (id > SENSOR_NUMBER)
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("<sensor>标签id数值超限异常"), id);
					return false;
				}
				pSen->SetId(id);

				//初始化IOSocket,凡是uintId==0的需要维护SOCKET
				if (pSen->GetUnitId() == 0)
				{
					//自己创建新的io
					pSen->InitIoSocket(nullptr);
				}
				else
				{
					pMainSen = pSenManager->GetMainSensor(ip, port);
					pSen->InitIoSocket(pMainSen->GetIoSocket());
				}
				//添加到容器
				pSenManager->pushSenElement(pSen->GetId(), pSen);
				//存入group Map；key为id。
				pGroup->pushElement(pSen->GetId(), pSen);
				//存入ipgroup Map；key为uid。
				pIpGroup->pushElement(pSen->GetUnitId(), pSen);
				//组id增1
				unitId++;
			/***********   离开<sensor>标签  *********************/
			}
			pGroup->pushIpGroup(pIpGroup->GetId(), pIpGroup);
			pIpGroup = nullptr;
			//IpGroup id 自增  每个Group内的IpGroup 都是从零开始自增的。
			ipGroupId++;
		/***********   离开<IpGroup>标签  *********************/
		}
		pSenManager->pushGrpElement(pGroup->GetId(), pGroup);
		pGroup = nullptr;
	/***********   离开<Group>标签  *********************/
	}
	//读完清除doc
	doc.Clear();
	//回收工厂
	SensorFactory::DestroyInstance();
	return true;
}


bool ConfigFileUtill::LoadLaserCaliFile(Calculate* pCalculate)
{
	CString filePath = MessageProcess::GetFilePath(_T("ini\\LaserCaliData.xml")); //文件名
	using namespace std;
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// 读入XML文件
	if (!doc.LoadFile(path))
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("读取标定文件失败，可能原因：配置文件格式错误、路径错误"));
		return false;
	}

	//开始解析
	TiXmlElement* root = doc.RootElement();//根标签
	if (root == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件root标签异常"));
		return false;
	}

	TiXmlElement* sElem = root->FirstChildElement();
	if (sElem == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件root标签异常"));
		return false;
	}

	/***********    进入根标签  *********************/
	int sid = 0; // 传感器id
	for (sElem; sElem != 0; sElem = sElem->NextSiblingElement())
	{
		if (sElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件<sensor>标签异常"), sid);
			return false;
		}

		//载入gene标定数据
		TiXmlElement* gcalElem = sElem->FirstChildElement("gCaliValue");
		if (gcalElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件<gCaliValue>标签异常"), sid);
			return false;
		}
		double gcaliValue = 0;
		int errorCode = gcalElem->QueryDoubleAttribute("value", &gcaliValue);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件标定数据属性异常！"), sid);
			return false;
		}
		//存储标定数据，
		pCalculate->SetGeneCaliValue(sid, gcaliValue);


		//载入标定数据
		TiXmlElement* rcalElem = sElem->FirstChildElement("rCaliValue");
		if (rcalElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件<rCaliValue>标签异常"), sid);
			return false;
		}
		double rcaliValue = 0;
		errorCode = rcalElem->QueryDoubleAttribute("value", &rcaliValue);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件标定数据属性异常！"), sid);
			return false;
		}
		//存储标定数据，
		pCalculate->SetRoofCaliValue(sid, rcaliValue);

		//载入gc高度标定数据
		TiXmlElement* gcheightElem = sElem->FirstChildElement("gcHeight");
		if (gcheightElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件<gcHeight>标签异常！"), sid);
			return false;
		}
		double gcHeightValue = 0;
		errorCode = gcheightElem->QueryDoubleAttribute("value", &gcHeightValue);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件标定数据属性异常！"), sid);
			return false;
		}
		//存储gc高度标定数据
		pCalculate->SetCaliGcHeight(sid, gcHeightValue);

		//载入rc高度标定数据
		TiXmlElement* rcheightElem = sElem->FirstChildElement("rcHeight");
		if (rcheightElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件<rcHeight>标签异常！"), sid);
			return false;
		}
		double rcHeightValue = 0;
		errorCode = rcheightElem->QueryDoubleAttribute("value", &rcHeightValue);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定文件标定数据属性异常！"), sid);
			return false;
		}
		//存储rc高度标定数据
		pCalculate->SetCaliRcHeight(sid, rcHeightValue);

		//id自增
		sid++;
	}
	//读完清除doc
	doc.Clear();
	return true;
}

bool ConfigFileUtill::WriteToCaliFile(int _id, double _attr, const CString sTitle)
{
	CString filePath = MessageProcess::GetFilePath(_T("ini\\LaserCaliData.xml")); //文件名
	using namespace std;
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// 读入XML文件
	if (!doc.LoadFile(path))
	{
		LOG(ERROR) << MessageProcess::ProcessMessage( _T("读取标定文件失败！可能原因：标定文件格式错误、路径错误等。"), _id);
		return false;
	}
	//开始解析
	TiXmlElement* root = doc.RootElement();//根标签
	if (root == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage( _T("标定文件root标签异常！"), _id);
		return false;
	}

	TiXmlElement* sElem = root->FirstChildElement();
	if (sElem == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage( _T("标定文件<sensor>标签异常！"), _id);
		return false;
	}
	/***********    进入<sensor>标签  *********************/
	bool isWritten = false; //跳出标志
	for (sElem; sElem != 0; sElem = sElem->NextSiblingElement())
	{
		int id = 0;
		sElem->QueryIntAttribute("id", &id);
		if (id == _id)
		{
			//写入数据到指定标签的对应属性中
			string strTitle = CT2A(sTitle);
			const char *title = strTitle.c_str();
			TiXmlElement* childElem = sElem->FirstChildElement(title);
			childElem->SetDoubleAttribute("value", _attr);
			isWritten = true; //跳出置为真
		}
		if (isWritten)
			break;//写完跳出
	}
	//改完写出
	bool iswritten = doc.SaveFile(path);
	if (!iswritten)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage( _T("标定文件保存失败！"), _id);
		return false;
	}
	//释放资源
	doc.Clear();
	return true;
}

