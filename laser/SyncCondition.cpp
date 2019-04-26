#include "stdafx.h"

#include "SyncCondition.h"

SyncCondition::SyncCondition()
{
	m_phEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
#if (_WIN32_WINNT >= 0x0403)
	//ʹ�� InitializeCriticalSectionAndSpinCount �����������
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

//�߳̽���ȴ�״̬Ҫ��ϵͳ���û�̬�л����ں�̬, �����ϴ󣡵��ǻ�û���ֺõĽ���취��ͬ��
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
	WaitForSingleObject(m_phEvent, INFINITE);//�ȴ���������Զ�ȴ�
	ResetEvent(m_phEvent);
}

void SyncCondition::Signal()
{
	SetEvent(m_phEvent);//����
}

void SyncCondition::ResetSignal(){
	ResetEvent(m_phEvent);
}


BOOL SyncCondition::Lock()
{
	::EnterCriticalSection(&m_CritSec);//�����ٽ���
	return TRUE;
}

BOOL SyncCondition::UnLock()
{
	::LeaveCriticalSection(&m_CritSec);//�뿪�ٽ���
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