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
	// ����XML�ļ�
	CString filePath = MessageProcess::GetFilePath(_T("ini\\LaserModuleIni.xml")); //�ļ���
	using namespace std;
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	if (!doc.LoadFile(path))
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("��ȡ�����ļ�ʧ�ܣ�����ԭ�������ļ���ʽ����·������"));
		return false;
	}

	Sensor *pSen = nullptr;
	Sensor *pMainSen = nullptr;
	SensorFactory* factory = SensorFactory::GetSensorFactory();
	Group* pGroup = nullptr;
	IpGroup* pIpGroup = nullptr;
	set<int> portSet; //port ���ʹ��
	CString brand;
	int port;
	int errorCode;
	CString ip;
	int id = 0; //������ID ��0������
	int groupId = 0;//Group ID ��0������
	/*
	for 1�������õ�ÿһ��<group>��ǩ
	for 2�������õ�ÿһ��<ipgroup>��ǩ
	for 3�������õ�ÿһ��<sensor>��ǩ
	*/
	//��ʼ����
	TiXmlElement* root = doc.RootElement();//����ǩ
	if (root == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�root��ǩ�쳣"));
		return false;
	}

	TiXmlElement* gElem = root->FirstChildElement();
	if (gElem == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�root��ǩ�쳣"));
		return false;
	}

	TiXmlElement* ipElem = NULL;
	TiXmlElement* sElem = NULL;
	TiXmlElement* calElem = NULL;
	TiXmlElement* gcheightElem = NULL;
	TiXmlAttribute *attr = NULL;

	/***********    ����<group>��ǩ  *********************/
	int loadedGroupId = -1;
	for (gElem; gElem != 0; gElem = gElem->NextSiblingElement())
	{
		pGroup = new Group();
		//����Group ID
		errorCode = gElem->QueryIntAttribute("id", &groupId);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<group>��ǩid���������쳣"));
			return false;
		}
		else if (groupId < 0 || groupId > 1)
		{   
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<ipgroup>��ǩid������ֵ��"));
			return false;
		}
		else if (groupId == loadedGroupId)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<ipgroup>��ǩid�����ظ�"));
			return false;
		}
		loadedGroupId = groupId;//������Group id
		pGroup->SetId(groupId);

		if (gElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<group>��ǩ�쳣"));
			return false;
		}
		/***********    ����<ipGroup>��ǩ  *********************/
		int ipGroupId = 0;
		ipElem = gElem->FirstChildElement();
		for (ipElem; ipElem != 0; ipElem = ipElem->NextSiblingElement())
		{
			if (ipElem == NULL)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<ipgroup>��ǩ�쳣"));
				return false;
			}
			//����һ��IpGroup����
			pIpGroup = new IpGroup();
			pIpGroup->SetId(ipGroupId);

			//ȡ��ip
			attr = ipElem->LastAttribute();
			ip = attr->Value();
			string str = CT2CA(ip);
			//����ipGroup Ip
			pIpGroup->SetIp(ip);
			//�����ipgroup����ʼsuId��
			int suId = 0;
			errorCode = ipElem->QueryIntAttribute("suid", &suId);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�suid�����쳣"), ipGroupId);
				return false;
			}
			else if (suId<0 || suId>8)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�suid��ֵ�쳣"), port);
				return false;
			}

			//��ȡport
			errorCode = ipElem->QueryIntAttribute("port", &port);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ��˿������쳣"), port);
				return false;
			}
			else if (port<1024)
			{
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ��˿���ֵ�쳣"), port);
				return false;
			}

			//���suid
			if (suId != 0 || portSet.count(port) != 0)
			{
				if (!pSenManager->JudgeUnitId(port, suId))
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<ipgroup>��ǩsuId��ֵ�쳣"), port);
					return false;
				}
			}

			//����ipGroup port
			pIpGroup->SetPort(port);
			portSet.insert(port);

			//�õ���һ��<sensor>��ǩ����ʼ����
			sElem = ipElem->FirstChildElement();
			int unitId = suId;
			/***********    ����<sensor>��ǩ  *********************/
			for (sElem; sElem != 0; sElem = sElem->NextSiblingElement())
			{
				if (sElem == NULL)
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<sensor>��ǩ�쳣"));
					return false;
				}

				//����Ʒ��
				brand = sElem->FirstChildElement("brand")->GetText();
				if (brand == _T("KEYENCE"))
				{
					pSen = factory->GetSensor(KEYENCE);//��ʼ������
					pSen->SetUnitId(unitId);//������unitId
					//������IsVice
					if (unitId == 0)
						pSen->SetIsVice(false);
					else
						pSen->SetIsVice(true);
					//������Brand
					pSen->SetBrand(KEYENCE);
				}
				else if (brand == _T("SICK"))
				{
					pSen = factory->GetSensor(SICK);
					pSen->SetBrand(SICK);
				}
				else{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<brand>��ǩ�쳣"), port);

					return false;
				}

				//����ip
				pSen->SetIp(ip);
				//����port
				pSen->SetPort(port);
				//����id
				errorCode = sElem->QueryIntAttribute("id", &id);
				if (errorCode == TIXML_WRONG_TYPE)
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<sensor>��ǩid���������쳣"), port);
					return false;
				}
				else if (pSenManager->ContainsSensor(id))
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("<sensor>��ǩid�ظ��쳣"), id);
					return false;
				}
				else if (id > SENSOR_NUMBER)
				{
					LOG(ERROR) << MessageProcess::ProcessMessage(_T("<sensor>��ǩid��ֵ�����쳣"), id);
					return false;
				}
				pSen->SetId(id);

				//��ʼ��IOSocket,����uintId==0����Ҫά��SOCKET
				if (pSen->GetUnitId() == 0)
				{
					//�Լ������µ�io
					pSen->InitIoSocket(nullptr);
				}
				else
				{
					pMainSen = pSenManager->GetMainSensor(ip, port);
					pSen->InitIoSocket(pMainSen->GetIoSocket());
				}
				//��ӵ�����
				pSenManager->pushSenElement(pSen->GetId(), pSen);
				//����group Map��keyΪid��
				pGroup->pushElement(pSen->GetId(), pSen);
				//����ipgroup Map��keyΪuid��
				pIpGroup->pushElement(pSen->GetUnitId(), pSen);
				//��id��1
				unitId++;
			/***********   �뿪<sensor>��ǩ  *********************/
			}
			pGroup->pushIpGroup(pIpGroup->GetId(), pIpGroup);
			pIpGroup = nullptr;
			//IpGroup id ����  ÿ��Group�ڵ�IpGroup ���Ǵ��㿪ʼ�����ġ�
			ipGroupId++;
		/***********   �뿪<IpGroup>��ǩ  *********************/
		}
		pSenManager->pushGrpElement(pGroup->GetId(), pGroup);
		pGroup = nullptr;
	/***********   �뿪<Group>��ǩ  *********************/
	}
	//�������doc
	doc.Clear();
	//���չ���
	SensorFactory::DestroyInstance();
	return true;
}


bool ConfigFileUtill::LoadLaserCaliFile(Calculate* pCalculate)
{
	CString filePath = MessageProcess::GetFilePath(_T("ini\\LaserCaliData.xml")); //�ļ���
	using namespace std;
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// ����XML�ļ�
	if (!doc.LoadFile(path))
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("��ȡ�궨�ļ�ʧ�ܣ�����ԭ�������ļ���ʽ����·������"));
		return false;
	}

	//��ʼ����
	TiXmlElement* root = doc.RootElement();//����ǩ
	if (root == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ�root��ǩ�쳣"));
		return false;
	}

	TiXmlElement* sElem = root->FirstChildElement();
	if (sElem == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ�root��ǩ�쳣"));
		return false;
	}

	/***********    �������ǩ  *********************/
	int sid = 0; // ������id
	for (sElem; sElem != 0; sElem = sElem->NextSiblingElement())
	{
		if (sElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ�<sensor>��ǩ�쳣"), sid);
			return false;
		}

		//����gene�궨����
		TiXmlElement* gcalElem = sElem->FirstChildElement("gCaliValue");
		if (gcalElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ�<gCaliValue>��ǩ�쳣"), sid);
			return false;
		}
		double gcaliValue = 0;
		int errorCode = gcalElem->QueryDoubleAttribute("value", &gcaliValue);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ��궨���������쳣��"), sid);
			return false;
		}
		//�洢�궨���ݣ�
		pCalculate->SetGeneCaliValue(sid, gcaliValue);


		//����궨����
		TiXmlElement* rcalElem = sElem->FirstChildElement("rCaliValue");
		if (rcalElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ�<rCaliValue>��ǩ�쳣"), sid);
			return false;
		}
		double rcaliValue = 0;
		errorCode = rcalElem->QueryDoubleAttribute("value", &rcaliValue);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ��궨���������쳣��"), sid);
			return false;
		}
		//�洢�궨���ݣ�
		pCalculate->SetRoofCaliValue(sid, rcaliValue);

		//����gc�߶ȱ궨����
		TiXmlElement* gcheightElem = sElem->FirstChildElement("gcHeight");
		if (gcheightElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ�<gcHeight>��ǩ�쳣��"), sid);
			return false;
		}
		double gcHeightValue = 0;
		errorCode = gcheightElem->QueryDoubleAttribute("value", &gcHeightValue);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ��궨���������쳣��"), sid);
			return false;
		}
		//�洢gc�߶ȱ궨����
		pCalculate->SetCaliGcHeight(sid, gcHeightValue);

		//����rc�߶ȱ궨����
		TiXmlElement* rcheightElem = sElem->FirstChildElement("rcHeight");
		if (rcheightElem == NULL)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ�<rcHeight>��ǩ�쳣��"), sid);
			return false;
		}
		double rcHeightValue = 0;
		errorCode = rcheightElem->QueryDoubleAttribute("value", &rcHeightValue);
		if (errorCode == TIXML_WRONG_TYPE)
		{
			LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨�ļ��궨���������쳣��"), sid);
			return false;
		}
		//�洢rc�߶ȱ궨����
		pCalculate->SetCaliRcHeight(sid, rcHeightValue);

		//id����
		sid++;
	}
	//�������doc
	doc.Clear();
	return true;
}

bool ConfigFileUtill::WriteToCaliFile(int _id, double _attr, const CString sTitle)
{
	CString filePath = MessageProcess::GetFilePath(_T("ini\\LaserCaliData.xml")); //�ļ���
	using namespace std;
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// ����XML�ļ�
	if (!doc.LoadFile(path))
	{
		LOG(ERROR) << MessageProcess::ProcessMessage( _T("��ȡ�궨�ļ�ʧ�ܣ�����ԭ�򣺱궨�ļ���ʽ����·������ȡ�"), _id);
		return false;
	}
	//��ʼ����
	TiXmlElement* root = doc.RootElement();//����ǩ
	if (root == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage( _T("�궨�ļ�root��ǩ�쳣��"), _id);
		return false;
	}

	TiXmlElement* sElem = root->FirstChildElement();
	if (sElem == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage( _T("�궨�ļ�<sensor>��ǩ�쳣��"), _id);
		return false;
	}
	/***********    ����<sensor>��ǩ  *********************/
	bool isWritten = false; //������־
	for (sElem; sElem != 0; sElem = sElem->NextSiblingElement())
	{
		int id = 0;
		sElem->QueryIntAttribute("id", &id);
		if (id == _id)
		{
			//д�����ݵ�ָ����ǩ�Ķ�Ӧ������
			string strTitle = CT2A(sTitle);
			const char *title = strTitle.c_str();
			TiXmlElement* childElem = sElem->FirstChildElement(title);
			childElem->SetDoubleAttribute("value", _attr);
			isWritten = true; //������Ϊ��
		}
		if (isWritten)
			break;//д������
	}
	//����д��
	bool iswritten = doc.SaveFile(path);
	if (!iswritten)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage( _T("�궨�ļ�����ʧ�ܣ�"), _id);
		return false;
	}
	//�ͷ���Դ
	doc.Clear();
	return true;
}

