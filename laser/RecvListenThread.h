#ifndef _RECVLISTENTHREAD_H_
#define _RECVLISTENTHREAD_H_
/**
* 该类继承于CThread，是一个数据接收监听类，主要监听传感器向上位机发送的数据、信息。
* @author zjs
* @Time 2017
*/

#include "Thread.h"
class SensorManager;

class RecvListenThread : public CThread
{
public:
	RecvListenThread();
	~RecvListenThread();
private:
	SensorManager* m_pSensorManager;

	// 事件句柄和套节字句柄表  
	WSAEVENT    eventArray[WSA_MAXIMUM_WAIT_EVENTS];

	SOCKET      sockArray[WSA_MAXIMUM_WAIT_EVENTS];

	int nEventTotal;

private:
	void Run();//线程工作区

public:
	void InitRecvListenThread(SensorManager* const _senMana);

};

#endif