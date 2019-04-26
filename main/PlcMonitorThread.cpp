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

		// ��ȡ��ǰʱ�䣬��������
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

	LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC����߳����˳�"));
}

void PlcMonitorThread::WriteTimer()
{
	// ����ͨѶ�ṹ������
	wplc_struct timerWplc;
	timerWplc.hasEnabled = false;
	timerWplc.szDevice = _T("D2049");
	timerWplc.lpsData = 0x1; //��Ϊ1
	//��ʱд��
	int wCode = m_pMxWrapper->write2Plc(&timerWplc);
	if (wCode != 0)
	{
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("��ʱ����д��PLCʧ��"));
	}
}

void PlcMonitorThread::ReadPlcMemory()
{
	//��ȡplc�ڴ���Ϣ
	rplc_struct rplc;
	int rCode = m_pMxWrapper->readPlc(&rplc);
	if (rCode != 0)
	{
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("��ȡPLC����ʧ��"));
		return;
	}

	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	//��ʾ�ƽ��͸���Ϣ��D2019~2024
	thrust_cylinder_struct tclds = m_pMxWrapper->getThrustCylinderStruct(&rplc);
	m_pMainDialog->m_DialogDisplayA->ShowThrustCylinderMsg(tclds);

	//��ʾ�͸���Ϣ��D2001~2006 D2011~2016
	cylinder_struct clds = m_pMxWrapper->getCylinderStruct(&rplc);
	m_pMainDialog->m_DialogDisplayB->ShowCylinderMsg(clds);

	//��ʾ������Ҫ��Ϣ D500��D512��D513��D516��
	m_pMainDialog->m_DialogDisplayB->ShowPlcMsg(rplc);
}