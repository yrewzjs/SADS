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

//定义

RangingModule::RangingModule()
{
}

RangingModule::~RangingModule()
{
}

bool RangingModule::InitRangingModule()
{
	// 初始化calculate
	m_pCalculate = new CalculateRanging();
	if (m_pCalculate->InitCalculate() == false)
	{
		return false;
	}

	//初始化SensorManager
	m_pSensorManager = new SensorManager();
	if (m_pSensorManager->InitSensorManager() == false)
	{
		return false;
	}

	//监听线程初始化
	m_pRecvListen = new RecvListenThread();
	m_pRecvListen->InitRecvListenThread(m_pSensorManager);
	m_pRecvListen->Start();

	//对所有传感器handshake检查
	CString failedId = _T("");
	bool isHake = m_pSensorManager->CheckConnectState(failedId);
	if (isHake)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("测距模块初始化成功"));
		return true;
	}
	else
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("SENSOR ") + failedId + _T(" 连接测试失败，请检查传感器连接线路"));
		return false;
	}
}

void RangingModule::UninitRangingModule()
{

	//运算完成，退出监听线程
	if (m_pRecvListen != nullptr)
	{
		m_pRecvListen->SetExit(true);
		m_pRecvListen->join(); //等待线程退出，按理说不应该主线程用wait
		//执行完毕，退出
		LOG(INFO) << MessageProcess::ProcessMessage(_T("传感器监听线程结束"));
	}

	//回收内存
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

	//m_pRecvListen 在 m_pSockManager 之前回收更合理
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
	//已采到数据，通知开始计算
	RequestGroupSensorData(segment, failedId);
	//运算
	m_pCalculate->CalculateProc(request, response);
	// 传输显示数据
	ranging_struct rst = m_pCalculate->getRangingStruct(segment);
	response->SetRangingStruct(rst); 
}

void RangingModule::RangingModuleCaliEntry(LaserRequest* const request, LaserResponse* const response)
{
	//读取传感器命令
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
							SensorMark markArr[num] = { A0, B0, C0, D0, E0}; //标定传感器编号
							caliSenNum = num;
							idArr = markArr;
							break;
	}
	case CALI_GENE_SPLICE:
	{
							 const int num = 6;
							 label = _T("gCaliValue");
							 SensorMark markArr[num] = { A1, A2, B1, B2, C1, C2 };//标定传感器编号
							 caliSenNum = num;
							 idArr = markArr;
							 break;
	}
	case CALI_ROOF_CATCH:
	{
							const int num = 4;
							label = _T("rCaliValue");
							SensorMark markArr[num] = { F0, G0, H0, J0 }; //标定传感器编号
							caliSenNum = num;
							idArr = markArr;
							break;
	}
	case CALI_ROOF_SPLICE:
	{
							 //待定
							 break;
	}
	case CALI_GENE_HEIGHT:
	{
							 const int num = 5;
							 label = _T("gcHeight");
							 SensorMark markArr[num] = { A0, B0, C0, D0, E0 }; //需要标定传感器编号
							 caliSenNum = num;
							 idArr = markArr;
							 break;
	}
	case CALI_ROOF_HEIGHT:
	{
							 const int num = 4;
							 label = _T("rcHeight");
							 SensorMark markArr[num] = { F0, G0, H0, J0 }; //标定传感器编号
							 caliSenNum = num;
							 idArr = markArr;
							 break;
	}
	}

	// 开始标定
	bool Flag = true;
	for (int i = 0; i < caliSenNum; i++)
	{
		int id = *(idArr++);
		double measData, writeData;
		RequestSensorData(id, measData);
		if (measData != SENSOR_REQ_ERROR_DATA)
		{
			if (label == _T("gCaliValue")) //和配置文件的标签名绑定的
			{
				m_pCalculate->SetMeasValue(id, measData);
				writeData = measData;
			}
			else if (label == _T("gcHeight"))
			{
				m_pCalculate->SetMeasValue(id, measData);
				writeData = m_pCalculate->GetGeneRealValue(id);
			}
			else if (label == _T("rCaliValue")) //和配置文件的标签名绑定的
			{
				m_pCalculate->SetMeasValue(id, measData);
				writeData = measData;
			}
			else if (label == _T("rcHeight"))
			{
				m_pCalculate->SetMeasValue(id, measData);
				writeData = m_pCalculate->GetRoofRealValue(id);
			}

			//写出到xml配置文件
			bool isWritten = ConfigFileUtill::WriteToCaliFile(id, writeData, label);
			//将标定数据写入日志中
			CString str = _T("");
			str.Format(_T("%s = %lf"), label, writeData);
			LOG(INFO) << MessageProcess::ProcessMessage(str, id);

			if (isWritten == false)
			{
				Flag = false;
				response->SetHasException(true);
				LOG(ERROR) << MessageProcess::ProcessMessage(_T("标定数据写入配置文件失败"),id);
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
		LOG(INFO) << MessageProcess::ProcessMessage(_T("测距模块标定命令成功执行"));
	}
	// 传输显示数据
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
		groupId = 1;  //roof组 id==1
	}
	else
	{
		groupId = 0;  //General组 groupid==0
	}

	bool returnFlag = true;
	CString idStr = _T("");
	//取出对应传感器组-general还是roof
	Group* pGroup = m_pSensorManager->GetGroup(groupId);
	if (pGroup == nullptr)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("GetGroup返回空指针"), groupId);
		return;
	}

	//由ipGroup id轮询 该分组下的ipGroup 获取组数据
	int ipGroId = 0;
	while (ipGroId != pGroup->GetIpGroupNum())
	{
		//取得ipGroup
		IpGroup* pIpGroup = pGroup->GetIpGroup(ipGroId);
		//取得该ipGroup 下的组数据
		CString groupData;
		pIpGroup->RequestIpGroupSensor(groupData);
		//将改组数据分配给对应的传感器，以传感器id存储
		map<int, Sensor*>::iterator iter = pIpGroup->GetIpSensorMap()->begin();
		while (iter != pIpGroup->GetIpSensorMap()->end())
		{
			Sensor* pSensor = iter->second;
			double validData = 0;
			pSensor->GetAndCheckSensorData(groupData, pSensor->GetUnitId(), validData);
			m_pCalculate->SetMeasValue(pSensor->GetId(), validData); //存入数据
			//迭代器后移
			iter++;
		}

		//内部ipGroup id是从零递增的
		ipGroId++;
	}
}

void RangingModule::RequestGroupSensorDataPoling(const SegmentAD segment, CString &failedId)
{
	int groupId = 0;
	if (segment == SEGAD_ROOF)
	{
		groupId = 1;  //roof组 id==1
	}
	else
	{
		groupId = 0;  //General组 groupid==0
	}

	CString validData;
	bool returnFlag = true;
	int FailedCount = 0;
	Sensor* pSensor = nullptr;
	CString idStr = _T("");
	//获取传感器数据
	Group* pGroup = m_pSensorManager->GetGroup(groupId);
	if (pGroup == nullptr)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("GetGroup返回空指针"), groupId);
		return;
	}

	map<int, Sensor*>* m_SensorMap = pGroup->GetSensorMap();
	map<int, Sensor*>::iterator iter = m_SensorMap->begin();
	while (iter != m_SensorMap->end())
	{
		pSensor = iter->second;
		//该传感器请求数据
		double calData = 0;
		pSensor->AcquireData(calData);
		m_pCalculate->SetMeasValue(pSensor->GetId(), calData);
		//迭代器后移一位
		iter++;
	}
}

