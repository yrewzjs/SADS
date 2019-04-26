#ifndef _THREADSYSNCONDITION_H_
#define _THREADSYSNCONDITION_H_
/**
* 该类是同步事件对象，主要是采用windows事件内核来保持数据采集同步，线程工作同步。
* @author zjs
* @Time 2017
*/

class SyncCondition
{
public:
	SyncCondition();
	~SyncCondition();

private:
	HANDLE m_phEvent; //同步事件句柄
	CRITICAL_SECTION m_CritSec;//临界区,关键段

public:
	BOOL Lock();//加锁，阻塞式
	BOOL UnLock();//解锁
	BOOL TryLock();//加锁，非阻塞式
public:
	DWORD Wait();
	DWORD Wait(int millisecond);
	void WaitForInfinite();
	void Signal();
	void ResetSignal();//置为未触发
};

#endif