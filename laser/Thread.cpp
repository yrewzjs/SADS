#include "stdafx.h"
#include <windows.h> 
#include <process.h> 
#include <string>
#include <time.h>

#include "Thread.h"
#include "SyncCondition.h"

CThread::CThread()
{
	m_pExitCond = new SyncCondition(); //������ false����ʼδ����
	m_ThreadState = THREAD_NEW; //�½�
	m_bExit = false;
}

CThread::CThread(char* name)
{
	m_ThreadName = name;
	m_pExitCond = new SyncCondition(); //������ false����ʼδ����
	m_ThreadState = THREAD_NEW; //�½�
	m_bExit = false;
}

CThread::~CThread()
{
	//if (m_hthreadHandle != NULL)
	//{
	//	//if (::FindClose(m_hthreadHandle))
	//		::CloseHandle(m_hthreadHandle);//�رվ��
	//}
	//	
	delete m_pExitCond;
}

HANDLE CThread::GetThreadHandle()
{
	return m_hthreadHandle;
}

void CThread::SetThreadHandle(HANDLE hdl)
{
	m_hthreadHandle = hdl;
}

void CThread::SetThreadState(ThreadState state)
{
	m_ThreadState = state;
}

ThreadState CThread::GetThreadState()
{
	return m_ThreadState;
}

//Start to execute the thread 
void CThread::Start()
{
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, this, 0, &m_ThreadID);
	CloseHandle(hThread); //���̴߳��������ϵ���CloseHandle()�Ǹ����õ�����
	//this->SetThreadHandle(hThread);
}

unsigned __stdcall CThread::ThreadFunction(void* pArg) //�̺߳������
{
	CThread* pThread = (CThread*)pArg;
	pThread->SetThreadState(THREAD_RUNNABLE);//����״̬

	//��������ʵ�ֵ�run()����
	pThread->Run();//�߳�ִ�д�����ڣ�run()ִ����������߳�����

	pThread->SetThreadState(THREAD_TERMINATED); //��������״̬
	return TRUE;
}

int  CThread::GetThreadID(void)
{
	return m_ThreadID;
}

void CThread::SetThreadName(char* threadName)
{
	m_ThreadName=threadName;
}

char* CThread::GetThreadName()
{
	return m_ThreadName;
}

void CThread::SetExit(bool _exit)
{
	m_bExit = _exit;
}

bool CThread::GetExit()
{
	return m_bExit;
}

//�ȴ����߳���ֹ��
void CThread::join()
{
	join(0);
}

void CThread::join(long sec)
{
	time_t tv, te;
	tv = time(NULL);
	long base = tv;
	long now = 0;
	if (sec < 0) {
		//"timeout value is negative"
	}

	if (sec == 0) {
		while (IsAlive()) {
			m_pExitCond->Wait(0);  //��������
			Sleep(100);
		}
	}
	else {
		while (IsAlive()) {
			long delay = sec - now;
			if (delay <= 0) {
				break;
			}
			m_pExitCond->Wait(delay);
			te = time(NULL);
			now = te - base;
		}
	}
}

bool CThread::IsAlive()
{
	if (m_ThreadState == THREAD_RUNNABLE)
		return true;
	else
		return false;
}