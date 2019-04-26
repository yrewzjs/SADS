#include "stdafx.h"
#include "LaserDriver.h"
#include "RangingModule.h"
#include "LaserResponse.h"
#include "SadsDef.h"
#include "MessageProcess.h"
#include "LaserDef.h"

LaserDriver::LaserDriver()
{
	m_isRangingInit = false;
}


LaserDriver::~LaserDriver()
{
	//若未主动调用UninitLaser()，则在析构函数中确保该方法被调用。
	UninitLaser();
}

bool LaserDriver::LoadWinsockDll()
{
	WORD wVersionRequested;
	/*
		WSADATA结构中填充WSAStartup载入的Winsock动态链库的信息
	*/
	WSADATA wsaData;
	int err;
	/**
		wVersionRequested指定了你想载入的Winsock版本,
		宏MAKEWORD(x, y)来指定版本号，这里x代表主版本，而y代表次版本
	*/
	wVersionRequested = MAKEWORD(2, 2);
	/*
		Winsock程序必须使用WSAStartup载入合适的Winsock动态链接库
	*/
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) 
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("没有可用的winsock DLL版本"));
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) 
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("winsock DLL版本不匹配"));
		WSACleanup();
		return false;
	}
	return true;
}

void LaserDriver::UnloadWinsockDll()
{
	/*
		使用完Winsock接口后，要调用int WSACleanup(void);函数对其占用的资源进行释放：
		调用该函数失败也没有什么问题，因为操作系统为自动将其释放，
		对应于每一个WSAStartup调用都应该有一个WSACleanup调用.
	*/
	WSACleanup();
}

bool LaserDriver::InitLaser()
{
	//加载2.2的DLL版本
	bool isLoad = LoadWinsockDll();
	if (!isLoad)
	{
		return false;
	}

#ifdef LASER_NOT_CONNECT
	return true;
#else
	if (!m_isRangingInit)
	{
		m_pRangingModule = new RangingModule();
		m_isRangingInit = m_pRangingModule->InitRangingModule();//初始化测距模块
	}
	return m_isRangingInit;
#endif
}

#ifdef LASER_NOT_CONNECT
	#include<stdlib.h>
	#include<time.h>
	#define random(x) (rand()%x - 200)
#endif

void LaserDriver::CallRangingAdjust(LaserRequest* const request, LaserResponse* const response)
{
#ifdef LASER_NOT_CONNECT
	response->SetIsFinished(true);
	ranging_struct ranging;
	srand((unsigned int)time(0));
	for (int i = 0; i < SENSOR_NUMBER; i++)
	{
		ranging.cali_v[i] = random(200); 
		ranging.meas_v[i] = random(200);
		ranging.real_v[i] = random(200);
	}
	response->SetRangingStruct(ranging);
#else
	if (m_isRangingInit)
	{
		//调用
		m_pRangingModule->RangingModuleAdjustEntry(request, response);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("测距模块未初始化，不能进行调整"));
	}
#endif
}

void LaserDriver::CallRangingCali(LaserRequest* const request, LaserResponse* const response)
{
#ifdef LASER_NOT_CONNECT
	ranging_struct ranging;
	srand((unsigned int)time(0));
	for (int i = 0; i < SENSOR_NUMBER; i++)
	{
		ranging.cali_v[i] = random(200); 
		ranging.meas_v[i] = random(200);
		ranging.real_v[i] = random(200);
	}
	response->SetRangingStruct(ranging);
#else
	if (m_isRangingInit)
	{
		//调用
		m_pRangingModule->RangingModuleCaliEntry(request, response);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("测距模块未初始化，不能进行标定"));
	}
#endif
}

void LaserDriver::UninitLaser()
{
#ifdef LASER_NOT_CONNECT
	//do nothing
#else
	if (m_pRangingModule != nullptr)
	{
		m_pRangingModule->UninitRangingModule();
		m_isRangingInit = false;
	}

	if (m_pRangingModule != nullptr)
	{
		delete m_pRangingModule;
		m_pRangingModule = nullptr; //置为空指针，C++11新特性
	}
#endif
	//卸载winsock DLL,应该放到最后
	UnloadWinsockDll();
	LOG(INFO) << MessageProcess::ProcessMessage(_T("测距模块反初始化完成"));
}

void LaserDriver::SetIsRangingInit(bool bl)
{
	m_isRangingInit = bl;
}

bool LaserDriver::GetIsRangingInit()
{
	return m_isRangingInit;
}

//如果不在这初始化的话就会出现链接错误error LNK2001
LaserDriver* LaserDriver::m_pInstance = nullptr;
LaserDriver* LaserDriver::GetInstance()
{
	///进入子线程关键区域
	//g_mutex.Lock();

	//懒惰形式实现的单例
	if (m_pInstance == nullptr)
		m_pInstance = new LaserDriver();

	//离开子线程关键区域
	//g_mutex.UnLock();
	return m_pInstance;
}

void LaserDriver::DestroyInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
