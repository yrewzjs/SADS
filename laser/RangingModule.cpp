#include "stdafx.h"
#include <string>
#include <map>

#include "RangingModule.h"
#include "MessageProcess.h"
#include "Calculate.h"
#include "RecvListenThread.h"
#include "SyncCondition.h"
#include "LaserResponse.h"
#include "CalculateRanging.h"
#include "LaserRequest.h"
#include "SensorManager.h"
#include "Sensor.h"

#include "Group.h"
#include "IpGroup.h"
#include "SadsDef.h"
#include "LaserDef.h"
#include "ConfigFileUtill.h"

//����

RangingModule::RangingModule()
{
}

RangingModule::~RangingModule()
{
}

bool RangingModule::InitRangingModule()
{
	// ��ʼ��calculate
	m_pCalculate = new CalculateRanging();
	if (m_pCalculate->InitCalculate() == false)
	{
		return false;
	}

	//��ʼ��SensorManager
	m_pSensorManager = new SensorManager();
	if (m_pSensorManager->InitSensorManager() == false)
	{
		return false;
	}

	//�����̳߳�ʼ��
	m_pRecvListen = new RecvListenThread();
	m_pRecvListen->InitRecvListenThread(m_pSensorManager);
	m_pRecvListen->Start();

	//�����д�����handshake���
	CString failedId = _T("");
	bool isHake = m_pSensorManager->CheckConnectState(failedId);
	if (isHake)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("���ģ���ʼ���ɹ�"));
		return true;
	}
	else
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("SENSOR ") + failedId + _T(" ���Ӳ���ʧ�ܣ����鴫����������·"));
		return false;
	}
}

void RangingModule::UninitRangingModule()
{

	//������ɣ��˳������߳�
	if (m_pRecvListen != nullptr)
	{
		m_pRecvListen->SetExit(true);
		m_pRecvListen->join(); //�ȴ��߳��˳�������˵��Ӧ�����߳���wait
		//ִ����ϣ��˳�
		LOG(INFO) << MessageProcess::ProcessMessage(_T("�����������߳̽���"));
	}

	//�����ڴ�
	if (m_pCalculate != nullptr)
	{
		delete m_pCalculate;
		m_pCalculate = nullptr;
	}

	if (m_pRecvListen != nullptr)
	{
		delete m_pRecvListen;
		m_pRecvListen = nullptr;
	}

	//m_pRecvListen �� m_pSockManager ֮ǰ���ո�����
	if (m_pSensorManager != nullptr)
	{
		delete m_pSensorManager;
		m_pSensorManager = nullptr;
	}
}

void RangingModule::RangingModuleAdjustEntry(LaserRequest* const request, LaserResponse* const response)
{
	CString failedId = _T("");
	SegmentAD segment = request->GetSegmentAD();
	//�Ѳɵ����ݣ�֪ͨ��ʼ����
	RequestGroupSensorData(segment, failedId);
	//����
	m_pCalculate->CalculateProc(request, response);
	// ������ʾ����
	ranging_struct rst = m_pCalculate->getRangingStruct(segment);
	response->SetRangingStruct(rst); 
}

void RangingModule::RangingModuleCaliEntry(LaserRequest* const request, LaserResponse* const response)
{
	//��ȡ����������
	CaliStep step = request->GetCaliStep();
	int caliSenNum = 0;
	CString label = _T("");
	SensorMark* idArr;
	switch (step)
	{
	case CALI_GENE_CATCH:
	{
							const int num = 5;
							label = _T("gCaliValue");
							SensorMark markArr[num] = { A0, B0, C0, D0, E0}; //�궨���������
							caliSenNum = num;
							idArr = markArr;
							break;
	}
	case CALI_GENE_SPLICE:
	{
							 const int num = 6;
							 label = _T("gCaliValue");
							 SensorMark markArr[num] = { A1, A2, B1, B2, C1, C2 };//�궨���������
							 caliSenNum = num;
							 idArr = markArr;
							 break;
	}
	case CALI_ROOF_CATCH:
	{
							const int num = 4;
							label = _T("rCaliValue");
							SensorMark markArr[num] = { F0, G0, H0, J0 }; //�궨���������
							caliSenNum = num;
							idArr = markArr;
							break;
	}
	case CALI_ROOF_SPLICE:
	{
							 //����
							 break;
	}
	case CALI_GENE_HEIGHT:
	{
							 const int num = 5;
							 label = _T("gcHeight");
							 SensorMark markArr[num] = { A0, B0, C0, D0, E0 }; //��Ҫ�궨���������
							 caliSenNum = num;
							 idArr = markArr;
							 break;
	}
	case CALI_ROOF_HEIGHT:
	{
							 const int num = 4;
							 label = _T("rcHeight");
							 SensorMark markArr[num] = { F0, G0, H0, J0 }; //�궨���������
							 caliSenNum = num;
							 idArr = markArr;
							 break;
	}
	}

	// ��ʼ�궨
	bool Flag = true;
	for (int i = 0; i < caliSenNum; i++)
	{
		int id = *(idArr++);
		double measData, writeData;
		RequestSensorData(id, measData);
		if (measData != SENSOR_REQ_ERROR_DATA)
		{
			if (label == _T("gCaliValue")) //�������ļ��ı�ǩ���󶨵�
			{
				m_pCalculate->SetMeasValue(id, measData);
				writeData = measData;
			}
			else if (label == _T("gcHeight"))
			{
				m_pCalculate->SetMeasValue(id, measData);
				writeData = m_pCalculate->GetGeneRealValue(id);
			}
			else if (label == _T("rCaliValue")) //�������ļ��ı�ǩ���󶨵�
			{
				m_pCalculate->SetMeasValue(id, measData);
				writeData = measData;
			}
			else if (label == _T("rcHeight"))
			{
				m_pCalculate->SetMeasValue(id, measData);
				writeData = m_pCalculate->GetRoofRealValue(id);
			}

			//д����xml�����ļ�
			bool isWritten = ConfigFileUtill::WriteToCaliFile(id, writeData, label);
			//���궨����д����־��
			CString str = _T("");
			str.Format(_T("%s = %lf"), label, writeData);
			LOG(INFO) << MessageProcess::ProcessMessage(str, id);

			if (isWritten == false)
			{
				Flag = false;
				response->SetHasException(true);
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("�궨����д�������ļ�ʧ��"),id);
			}
		}
		else
		{
			Flag = false;
			response->SetHasException(true);
		}
	}

	if (Flag == true)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("���ģ��궨����ɹ�ִ��"));
	}
	// ������ʾ����
	ranging_struct rst = m_pCalculate->getRangingStruct(request->GetSegmentAD());
	response->SetRangingStruct(rst);
}

void RangingModule::RequestSensorData(int sId)
{
	Sensor* pSensor = m_pSensorManager->GetSensor(sId);
	double validData = 0;
	pSensor->AcquireData(validData);
	m_pCalculate->SetMeasValue(pSensor->GetId(), validData);
}

void RangingModule::RequestSensorData(const int sId, double & data)
{
	Sensor* pSensor = m_pSensorManager->GetSensor(sId);
	pSensor->AcquireData(data);
}

void RangingModule::RequestGroupSensorData(const SegmentAD segment, CString &failedId)
{
	int groupId = 0;
	if (segment == SEGAD_ROOF)
	{
		groupId = 1;  //roof�� id==1
	}
	else
	{
		groupId = 0;  //General�� groupid==0
	}

	bool returnFlag = true;
	CString idStr = _T("");
	//ȡ����Ӧ��������-general����roof
	Group* pGroup = m_pSensorManager->GetGroup(groupId);
	if (pGroup == nullptr)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("GetGroup���ؿ�ָ��"), groupId);
		return;
	}

	//��ipGroup id��ѯ �÷����µ�ipGroup ��ȡ������
	int ipGroId = 0;
	while (ipGroId != pGroup->GetIpGroupNum())
	{
		//ȡ��ipGroup
		IpGroup* pIpGroup = pGroup->GetIpGroup(ipGroId);
		//ȡ�ø�ipGroup �µ�������
		CString groupData;
		pIpGroup->RequestIpGroupSensor(groupData);
		//���������ݷ������Ӧ�Ĵ��������Դ�����id�洢
		map<int, Sensor*>::iterator iter = pIpGroup->GetIpSensorMap()->begin();
		while (iter != pIpGroup->GetIpSensorMap()->end())
		{
			Sensor* pSensor = iter->second;
			double validData = 0;
			pSensor->GetAndCheckSensorData(groupData, pSensor->GetUnitId(), validData);
			m_pCalculate->SetMeasValue(pSensor->GetId(), validData); //��������
			//����������
			iter++;
		}

		//�ڲ�ipGroup id�Ǵ��������
		ipGroId++;
	}
}

void RangingModule::RequestGroupSensorDataPoling(const SegmentAD segment, CString &failedId)
{
	int groupId = 0;
	if (segment == SEGAD_ROOF)
	{
		groupId = 1;  //roof�� id==1
	}
	else
	{
		groupId = 0;  //General�� groupid==0
	}

	CString validData;
	bool returnFlag = true;
	int FailedCount = 0;
	Sensor* pSensor = nullptr;
	CString idStr = _T("");
	//��ȡ����������
	Group* pGroup = m_pSensorManager->GetGroup(groupId);
	if (pGroup == nullptr)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("GetGroup���ؿ�ָ��"), groupId);
		return;
	}

	map<int, Sensor*>* m_SensorMap = pGroup->GetSensorMap();
	map<int, Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSensor = iter->second;
		//�ô�������������
		double calData = 0;
		pSensor->AcquireData(calData);
		m_pCalculate->SetMeasValue(pSensor->GetId(), calData);
		//����������һλ
		iter++;
	}
}

