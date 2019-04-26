#ifndef _RECVLISTENTHREAD_H_
#define _RECVLISTENTHREAD_H_
/**
* ����̳���CThread����һ�����ݽ��ռ����࣬��Ҫ��������������λ�����͵����ݡ���Ϣ��
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

	// �¼�������׽��־����  
	WSAEVENT    eventArray[WSA_MAXIMUM_WAIT_EVENTS];

	SOCKET      sockArray[WSA_MAXIMUM_WAIT_EVENTS];

	int nEventTotal;

private:
	void Run();//�̹߳�����

public:
	void InitRecvListenThread(SensorManager* const _senMana);

};

#endif