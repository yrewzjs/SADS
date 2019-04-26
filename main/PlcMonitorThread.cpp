#include "stdafx.h"
#include <windows.h>

#include "PlcMonitorThread.h"
#include "WorkingThread.h"
#include "MessageProcess.h"
#include "MxWrapper.h"
#include "MXComonentDef.h"
#include "SadsDef.h"

#include "SADSProject.h"
#include "SADSProjectDlg.h"
#include "DialogDisplayPartA.h"
#include "DialogDisplayPartB.h"

PlcMonitorThread::PlcMonitorThread(MxWrapper* pMxWrapper)
{
	m_pMxWrapper = pMxWrapper;
}

PlcMonitorThread::~PlcMonitorThread()
{
	
}

void PlcMonitorThread::Run()
{
	const clock_t wInterval = 1000L;
	const clock_t rInterval = 3000L;
	clock_t lastWriteTime,lastReadTime,currTime;
	lastWriteTime = lastReadTime = currTime = clock();
	while (true)
	{
		if (GetExit() == true)
		{
			break;
		}

		// 获取当前时间，毫秒量级
		currTime = clock();

		if (currTime - lastWriteTime >= wInterval)
		{
			this->WriteTimer();
			lastWriteTime = currTime;
		}

		if (currTime - lastReadTime >= rInterval)
		{
			this->ReadPlcMemory();
			lastReadTime = currTime;
		}
	}

	LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC监控线程已退出"));
}

void PlcMonitorThread::WriteTimer()
{
	// 保持通讯结构体内容
	wplc_struct timerWplc;
	timerWplc.hasEnabled = false;
	timerWplc.szDevice = _T("D2049");
	timerWplc.lpsData = 0x1; //置为1
	//定时写入
	int wCode = m_pMxWrapper->write2Plc(&timerWplc);
	if (wCode != 0)
	{
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("定时数据写入PLC失败"));
	}
}

void PlcMonitorThread::ReadPlcMemory()
{
	//读取plc内存信息
	rplc_struct rplc;
	int rCode = m_pMxWrapper->readPlc(&rplc);
	if (rCode != 0)
	{
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("读取PLC数据失败"));
		return;
	}

	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	//显示推进油缸信息，D2019~2024
	thrust_cylinder_struct tclds = m_pMxWrapper->getThrustCylinderStruct(&rplc);
	m_pMainDialog->m_DialogDisplayA->ShowThrustCylinderMsg(tclds);

	//显示油缸信息，D2001~2006 D2011~2016
	cylinder_struct clds = m_pMxWrapper->getCylinderStruct(&rplc);
	m_pMainDialog->m_DialogDisplayB->ShowCylinderMsg(clds);

	//显示其他必要信息 D500、D512、D513、D516、
	m_pMainDialog->m_DialogDisplayB->ShowPlcMsg(rplc);
}