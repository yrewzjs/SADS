#include "stdafx.h"
#include <windows.h> 
#include <process.h> 
#include <string>
#include <time.h>

#include "Thread.h"
#include "SyncCondition.h"

CThread::CThread()
{
	m_pExitCond = new SyncCondition(); //第三个 false：初始未触发
	m_ThreadState = THREAD_NEW; //新建
	m_bExit = false;
}

CThread::CThread(char* name)
{
	m_ThreadName = name;
	m_pExitCond = new SyncCondition(); //第三个 false：初始未触发
	m_ThreadState = THREAD_NEW; //新建
	m_bExit = false;
}

CThread::~CThread()
{
	//if (m_hthreadHandle != NULL)
	//{
	//	//if (::FindClose(m_hthreadHandle))
	//		::CloseHandle(m_hthreadHandle);//关闭句柄
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
	CloseHandle(hThread); //在线程创建后马上调用CloseHandle()是个良好的做法
	//this->SetThreadHandle(hThread);
}

unsigned __stdcall CThread::ThreadFunction(void* pArg) //线程函数入口
{
	CThread* pThread = (CThread*)pArg;
	pThread->SetThreadState(THREAD_RUNNABLE);//运行状态

	//调用子类实现的run()方法
	pThread->Run();//线程执行代码入口，run()执行完表明该线程死亡

	pThread->SetThreadState(THREAD_TERMINATED); //处于死亡状态
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

//等待该线程终止。
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
			m_pExitCond->Wait(0);  //立即返回
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