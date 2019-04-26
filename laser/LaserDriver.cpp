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
	//��δ��������UninitLaser()����������������ȷ���÷��������á�
	UninitLaser();
}

bool LaserDriver::LoadWinsockDll()
{
	WORD wVersionRequested;
	/*
		WSADATA�ṹ�����WSAStartup�����Winsock��̬�������Ϣ
	*/
	WSADATA wsaData;
	int err;
	/**
		wVersionRequestedָ�������������Winsock�汾,
		��MAKEWORD(x, y)��ָ���汾�ţ�����x�������汾����y����ΰ汾
	*/
	wVersionRequested = MAKEWORD(2, 2);
	/*
		Winsock�������ʹ��WSAStartup������ʵ�Winsock��̬���ӿ�
	*/
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) 
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("û�п��õ�winsock DLL�汾"));
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) 
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("winsock DLL�汾��ƥ��"));
		WSACleanup();
		return false;
	}
	return true;
}

void LaserDriver::UnloadWinsockDll()
{
	/*
		ʹ����Winsock�ӿں�Ҫ����int WSACleanup(void);��������ռ�õ���Դ�����ͷţ�
		���øú���ʧ��Ҳû��ʲô���⣬��Ϊ����ϵͳΪ�Զ������ͷţ�
		��Ӧ��ÿһ��WSAStartup���ö�Ӧ����һ��WSACleanup����.
	*/
	WSACleanup();
}

bool LaserDriver::InitLaser()
{
	//����2.2��DLL�汾
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
		m_isRangingInit = m_pRangingModule->InitRangingModule();//��ʼ�����ģ��
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
		//����
		m_pRangingModule->RangingModuleAdjustEntry(request, response);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("���ģ��δ��ʼ�������ܽ��е���"));
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
		//����
		m_pRangingModule->RangingModuleCaliEntry(request, response);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("���ģ��δ��ʼ�������ܽ��б궨"));
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
		m_pRangingModule = nullptr; //��Ϊ��ָ�룬C++11������
	}
#endif
	//ж��winsock DLL,Ӧ�÷ŵ����
	UnloadWinsockDll();
	LOG(INFO) << MessageProcess::ProcessMessage(_T("���ģ�鷴��ʼ�����"));
}

void LaserDriver::SetIsRangingInit(bool bl)
{
	m_isRangingInit = bl;
}

bool LaserDriver::GetIsRangingInit()
{
	return m_isRangingInit;
}

//����������ʼ���Ļ��ͻ�������Ӵ���error LNK2001
LaserDriver* LaserDriver::m_pInstance = nullptr;
LaserDriver* LaserDriver::GetInstance()
{
	///�������̹߳ؼ�����
	//g_mutex.Lock();

	//������ʽʵ�ֵĵ���
	if (m_pInstance == nullptr)
		m_pInstance = new LaserDriver();

	//�뿪���̹߳ؼ�����
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
