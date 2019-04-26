#ifndef _PLCMONITORTHREAD_H_
#define _PLCMONITORTHREAD_H_

#include "Thread.h"

class PlcMonitorThread:public CThread
{
public:
	PlcMonitorThread(class MxWrapper* pMxWrapper);
	~PlcMonitorThread();

private:
	class MxWrapper* m_pMxWrapper; // PLC模块句柄

public:
	void Run(); //线程入口

private:
	void WriteTimer();
	void ReadPlcMemory();
};

#endif