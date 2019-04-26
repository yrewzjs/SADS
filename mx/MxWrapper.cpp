#include "stdafx.h"

#include <bitset>
using std::bitset;

#include "MxWrapper.h"
#include "CActUtlType.h"
#include "MXComonentDef.h"
#include "SadsDef.h"
#include "Synchronized.h"

#ifdef PLC_NOT_CONNECT
	#include<stdlib.h>
	#include<time.h>
	#define random(x) (rand()%x)
#endif

#define DEVICE_NAME_501     _T("D501")  // 下发精定位调整状态
#define DEVICE_NAME_503     _T("D503")  // 写入油缸动作
#define DEVICE_NAME_504     _T("D504")  // 写入油缸动作方向


#define DEVICE_NAME_506     _T("D506")  // 写入油缸动作距离
#define DEVICE_NAME_507     _T("D507")  // 写入油缸动作旋转角度
#define DEVICE_NAME_510     _T("D510")  // 读取相机控制油缸动作完成情况

#define DEVICE_NAME_511     _T("D511")  // 下发精定位异常状态
#define DEVICE_NAME_514     _T("D514")  // 下发上位机状态

#define DEVICE_NAME_2025    _T("D2025") // 当前管片

MxWrapper::MxWrapper()
{
	m_pActUtl = nullptr;
	m_pSynchronized = new Synchronized();
}


MxWrapper::~MxWrapper()
{
	if (m_pSynchronized != nullptr)
	{
		delete m_pSynchronized;
		m_pSynchronized = nullptr;
	}
}

//CActUtlType* MxWrapper::m_pActUtl = nullptr; //定义
void MxWrapper::SetActUtlMx(CActUtlType* pAct)
{
	if (m_pActUtl == nullptr)
	{
		m_pActUtl = pAct; 
	}
}
/**
	func：初始化PLC通信控件
	@paramn：LogicNumber 逻辑站号
	@return：0代表成功，非0为失败
*/
long MxWrapper::initActUtl(long nLogicNumber)
{
#ifdef PLC_NOT_CONNECT
	long nCode = 0;
#else
	m_pActUtl->put_ActLogicalStationNumber(nLogicNumber);
	long nCode = m_pActUtl->Open();
#endif
	return nCode;
}

/*
	func：反初始化PLC通信控件
	@return：0代表成功，非0为失败
*/
long MxWrapper::uninitActUtl()
{
#ifdef PLC_NOT_CONNECT
	long nCode = 0;
#else
	long nCode = m_pActUtl->Close();
#endif
	return nCode;
}

/*
	func：读取PLC内存信息
	@param：rplc 存储内存信息结构体
	@return：0代表成功读取，非0为读取失败
*/
long MxWrapper::readPlc(rplc_struct *rplc)
{
	// 读不加锁
#ifdef PLC_NOT_CONNECT

	srand((unsigned int)time(0));
	for (int i = 0; i < DEVICE_D5XX_NUMBER; i++)
	{
		rplc->lpsData_D5xx[i] = random(4);
	}
	for (int i = 0; i < DEVICE_D2XXX_NUMBER; i++)
	{
		rplc->lpsData_D2xxx[i] = random(4);
	}

	int nCode = 0;
#else
	// 检查传入结构体是否为空
	if (rplc == nullptr)
	{
		return 1;
	}

	long err5 = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_500, DEVICE_D5XX_NUMBER, rplc->lpsData_D5xx);
	long err2 = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_2000, DEVICE_D2XXX_NUMBER, rplc->lpsData_D2xxx);
	long nCode = err2 + err5;
#endif
	return nCode;
}

/*
	func：向PLC内存写入指令
	@param：wplc 存储写入内存指令的结构体
	@return：0代表成功写入，非0为写入失败
*/
long MxWrapper::write2Plc(const wplc_struct *wplc)
{
	// 写加锁
	m_pSynchronized->Lock();

#ifdef PLC_NOT_CONNECT
	int nCode = 0;
#else
	// 检查传入结构体是否为空
	if (wplc == nullptr)
	{
		return 1;
	}

	// 检查命令是否有 使能位
	if (wplc->hasEnabled)
	{
		// 若有使能位，写入使能位命令
		short enabled_lpsData = wplc->enabled_lpsData;
		long neCode = m_pActUtl->WriteDeviceBlock2(wplc->enabled_szDevice, wplc->enabled_lSize, &enabled_lpsData);

		// 若写入使能位失败，直接返回
		if (neCode != 0)
		{
			return neCode;
		}
	}

	// 写入具体命令
	short lpsData = wplc->lpsData;
	long nCode = m_pActUtl->WriteDeviceBlock2(wplc->szDevice, wplc->lSize, &lpsData);
#endif
	// 解锁
	m_pSynchronized->UnLock();
	return nCode;
}

/*******************************************/
thrust_cylinder_struct MxWrapper::getThrustCylinderStruct(struct rplc_struct *rplc)
{
#ifdef PLC_NOT_CONNECT
	thrust_cylinder_struct tclds;
	srand((unsigned int)time(0));
	for (int i = 0; i < THRUST_CYLINDER_NUMBER; i++)
	{
		tclds.cylin_action[i] = random(4);
	}
	return tclds;
#else
	thrust_cylinder_struct tclds;
	//实现
	//先从所有PLC状态信息中提取推进油缸信息(D2019-D2024，共6个命令)，结果为short值
	const int CommdNumber = 6;    //推进油缸相关命令共6个
	const int CylinderNumer_0 = 16;
	const int CylinderNumer_1 = 3;
	const int FirstCommand = 19;  //推进油缸信息命令从D2019开始
	short thrust_cylinder_Valve[CommdNumber];
	
	//把对应的推进油缸数据导出来
	for (int i = 0; i < CommdNumber; i++)
	{
		thrust_cylinder_Valve[i] = rplc->lpsData_D2xxx[i + FirstCommand];
	}

	//对提取到的short值进行按位提取，并将提取结果按位赋予各油缸信息参量

	//第一组为D2019和D2020命令，确定各油缸是否是“伸”的状态
	bitset<16>Cylinder_number_0(thrust_cylinder_Valve[0]);
	bitset<16>Cylinder_number_1(thrust_cylinder_Valve[1]);
	//第二租为D2021和D2022命令，确定各油缸是否是“缩”的状态
	bitset<16>Cylinder_number_2(thrust_cylinder_Valve[2]);
	bitset<16>Cylinder_number_3(thrust_cylinder_Valve[3]);
	//第三组为D2023和D2024命令，确定各油缸是否达到缩限位位置
	bitset<16>Cylinder_number_4(thrust_cylinder_Valve[4]);
	bitset<16>Cylinder_number_5(thrust_cylinder_Valve[5]);

	//先判断1-16号油缸状态
	for (int i = 0; i < CylinderNumer_0; i++)
	{
		if (Cylinder_number_0[i])
		{
			tclds.cylin_action[i] = 1;//油缸状态为“伸”
		}
		else if (Cylinder_number_2[i])
		{
			tclds.cylin_action[i] = 2;//油缸状态为“缩”
		}
		else if (Cylinder_number_4[i])
		{
			tclds.cylin_action[i] = 3;//油缸到达缩限位
		}
		else
		{
			tclds.cylin_action[i] = 0;//油缸状态为“停”
		}
	}
	//判断17-19号油缸状态
	for (int j = 0; j < CylinderNumer_1; j++)
	{
		int CylinderNumber = j + CylinderNumer_0;
		if (Cylinder_number_1[j])
		{
			tclds.cylin_action[CylinderNumber] = 1;//油缸状态为“伸”
		}
		else if (Cylinder_number_3[j])
		{
			tclds.cylin_action[CylinderNumber] = 2;//油缸状态为“缩”
		}
		else if (Cylinder_number_5[j])
		{
			tclds.cylin_action[CylinderNumber] = 3;//油缸到达缩限位
		}
		else
		{
			tclds.cylin_action[CylinderNumber] = 0;//油缸状态为“停”
		}
	}
	return tclds;
#endif
}

cylinder_struct MxWrapper::getCylinderStruct(struct rplc_struct *rplc)
{
#ifdef PLC_NOT_CONNECT
	cylinder_struct tclds;
	srand((unsigned int)time(0));
	for (int i = 0; i < CYLINDER_NUMBER; i++)
	{
		tclds.cylin_action[i] = random(2);
		tclds.cylin_loc[i] = random(100);
	}
	return tclds;
#else
	cylinder_struct clds;
	//实现
	const int CommdNumber = 6;          //普通油缸动作信息命令和位置信息命令分别都是6个，分别为D2011-D2016和D2001-D2006
	const int FirstCommandAction = 11;  //普通油缸动作信息命令从D2011开始
	const int FirstCommandLocation = 1; //普通油缸位置信息命令从D2001开始
	for (int i = 0; i < CommdNumber; i++)
	{
		clds.cylin_action[i] = rplc->lpsData_D2xxx[i + FirstCommandAction];
		clds.cylin_loc[i] = rplc->lpsData_D2xxx[i + FirstCommandLocation];
	}
	return clds;
#endif
}

// 仅读取D500标志位,为PLC上发抓取拼装命令
long MxWrapper::readBitD500(short &D500)
{
#ifdef PLC_NOT_CONNECT
	D500 = 0; // plc上发
	return 0;
#else
	long rCode = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_500, 1, &D500);
	return rCode;
#endif
}

// 仅读取D2025标志位,为PLC上发当前管片
long MxWrapper::readBitD2025(short &D2025)
{
#ifdef PLC_NOT_CONNECT
	D2025 = 1;
	return 0;
#else
	long rCode = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_2025, 1, &D2025);
	return rCode;
#endif
}

// 下发精定位调整状态
long MxWrapper::writeBitD501(short D501)
{
	// 写加锁
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	long rCode = 0;
#else
	long rCode = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_501, 1, &D501);
#endif
	// 解锁
	m_pSynchronized->UnLock();
	return rCode;
}

// 下发精定位未完成
long MxWrapper::writeBitD511(short D511)
{
	// 写加锁
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	long rCode = 0;
#else
	long rCode = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_511, 1, &D511);
	
#endif
	// 解锁
	m_pSynchronized->UnLock();
	return rCode;
}

// 下发上位机状态
long MxWrapper::writeBitD514(short D514)
{
	// 写加锁
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	long rCode = 0;
#else
	long rCode = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_514, 1, &D514);
#endif
	// 解锁
	m_pSynchronized->UnLock();
	return rCode;
}

long MxWrapper::writeCylinderAction(const CylinderAD cld, const Direction dir)
{
	// 写加锁
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	return 0;
#else
	short D503 = 0;
	switch (cld)
	{
	case CLD_RED:
		D503 = 4;
		break;
	case CLD_BLUE:
		D503 = 5;
		break;
	case CLD_REDBLUE:
		D503 = 3;
		break;
	case CLD_SLIDE:
		D503 = 1;
		break;
	case CLD_ROTATE:
		D503 = 2;
		break;
	case CLD_PITCH:
		D503 = 6;
		break;
	case CLD_ROLL:
		D503 = 7;
		break;
	}

	short D504 = 0;
	switch (dir)
	{
	case KEEP:
		D504 = 0;
		break;
	case STRETCH:
		D504 = 1;
		break;
	case WITHDREW:
		D504 = 2;
		break;
	}



	if (D503 == 6)
	{
		short D506 = 2;
		short D507 = 50;
		short D510 = 0;

		//需要先将D503写为零
		short D503_0 = 0;
		long code1 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503_0);
		Sleep(50);

		//动作距离
		long code4 = 1;
		if (cld == CLD_ROTATE)
		{
			code4 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_507, 1, &D507);
		}
		else
		{
			code4 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_506, 1, &D506);
		}
		// 哪个油缸
		long code2 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503);
		// 哪个方向
		long code3 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_504, 1, &D504);

		Sleep(500);
		//读取动作成功与否
		long rD510 = 1;
		long rCode = 0;
		for (int i = 0; i < 95000; i += 100)
		{
			long rCode = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_510, 1, &D510);
			//动作是否完成
			if (D510 == 1)
			{
				rD510 = 0;
				break;
			}
		}
		// 解锁
		m_pSynchronized->UnLock();
		return code1 + code2 + code3 + code4 + rCode + rD510;
	}
	else
	{
		//一、需要先将D503写为零
		short D503_0 = 0;
		long code0 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503_0);
		//二、需要先将D506或者D507写为零
		long code1 = 0;
		if (cld == CLD_ROTATE)
		{
			short D507 = 0;
			code1 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_507, 1, &D507);
		}
		else
		{
			short D506 = 0;
			code1 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_506, 1, &D506);
		}
		//三、 哪个油缸
		long code2 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503);
		//四、 哪个方向
		long code3 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_504, 1, &D504);
		// 解锁
		m_pSynchronized->UnLock();
		return code0 + code1 + code2 + code3;
	}

	
#endif
}

long MxWrapper::writeCylinderAction2(CylinderAD cld, Direction dir)
{
	// 写加锁
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	m_pSynchronized->UnLock();
	return 0;
#else
	short D503 = 0;
	switch (cld)
	{
	case CLD_RED:
		D503 = 4;
		break;
	case CLD_BLUE:
		D503 = 5;
		break;
	case CLD_REDBLUE:
		D503 = 3;
		break;
	case CLD_SLIDE:
		D503 = 1;
		break;
	case CLD_ROTATE:
		D503 = 2;
		break;
	case CLD_PITCH:
		D503 = 6;
		break;
	case CLD_ROLL:
		D503 = 7;
		break;
	}

	short D504 = 0;
	switch (dir)
	{
	case KEEP:
		D504 = 0;
		break;
	case STRETCH:
		D504 = 1;
		break;
	case WITHDREW:
		D504 = 2;
		break;
	}

	short D506 = 2;
	short D507 = 50;
	short D510 = 0;

	//需要先将D503写为零
	short D503_0 = 0;
	long code1 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503_0);
	Sleep(50);

	//动作距离
	long code4 = 1;
	if (cld == CLD_ROTATE)
	{
		code4 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_507, 1, &D507);
	}
	else
	{
		code4 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_506, 1, &D506);
	}
	// 哪个油缸
	long code2 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503);
	// 哪个方向
	long code3 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_504, 1, &D504);

	Sleep(500);
	//读取动作成功与否
	long rD510 = 1;
	long rCode = 0;
	for (int i = 0; i < 95000; i += 100)
	{
		long rCode = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_510, 1, &D510);
		//动作是否完成
		if (D510 == 1)
		{
			rD510 = 0;
			break;
		}
	}


	// 解锁
	m_pSynchronized->UnLock();
	return code1 + code2 + code3 + code4 + rCode + rD510;
#endif
}


