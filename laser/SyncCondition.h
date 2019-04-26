#ifndef _THREADSYSNCONDITION_H_
#define _THREADSYSNCONDITION_H_
/**
* ������ͬ���¼�������Ҫ�ǲ���windows�¼��ں����������ݲɼ�ͬ�����̹߳���ͬ����
* @author zjs
* @Time 2017
*/

class SyncCondition
{
public:
	SyncCondition();
	~SyncCondition();

private:
	HANDLE m_phEvent; //ͬ���¼����
	CRITICAL_SECTION m_CritSec;//�ٽ���,�ؼ���

public:
	BOOL Lock();//����������ʽ
	BOOL UnLock();//����
	BOOL TryLock();//������������ʽ
public:
	DWORD Wait();
	DWORD Wait(int millisecond);
	void WaitForInfinite();
	void Signal();
	void ResetSignal();//��Ϊδ����
};

#endif