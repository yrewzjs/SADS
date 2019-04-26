#include "stdafx.h"

#include "SyncCondition.h"

SyncCondition::SyncCondition()
{
	m_phEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
#if (_WIN32_WINNT >= 0x0403)
	//使用 InitializeCriticalSectionAndSpinCount 可以提高性能
	::InitializeCriticalSectionAndSpinCount(&m_CritSec, 4000);
#else
	::InitializeCriticalSection(&m_CritSec);
#endif
}

SyncCondition::~SyncCondition()
{
	if (NULL != m_phEvent)
	{
		::CloseHandle((m_phEvent));
	}

	::DeleteCriticalSection(&m_CritSec);
}

//线程进入等待状态要被系统从用户态切换到内核态, 开销较大！但是还没发现好的解决办法能同步
DWORD SyncCondition::Wait()
{
	DWORD wait = WaitForSingleObject(m_phEvent, 2000);
	ResetEvent(m_phEvent);
	return wait;
}

DWORD SyncCondition::Wait(int millisecond){
	DWORD wait = WaitForSingleObject(m_phEvent, millisecond);
	ResetEvent(m_phEvent);
	return wait;
}

void SyncCondition::WaitForInfinite()
{
	WaitForSingleObject(m_phEvent, INFINITE);//等待函数，永远等待
	ResetEvent(m_phEvent);
}

void SyncCondition::Signal()
{
	SetEvent(m_phEvent);//发信
}

void SyncCondition::ResetSignal(){
	ResetEvent(m_phEvent);
}


BOOL SyncCondition::Lock()
{
	::EnterCriticalSection(&m_CritSec);//进入临界区
	return TRUE;
}

BOOL SyncCondition::UnLock()
{
	::LeaveCriticalSection(&m_CritSec);//离开临界区
	return TRUE;
}

BOOL SyncCondition::TryLock()
{
	while (true)
	{
		BOOL bRet = TryEnterCriticalSection(&m_CritSec);
		if (bRet)
			break;
	}
	return TRUE;
}