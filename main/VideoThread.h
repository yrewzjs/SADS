#ifndef _VIDEOTHREAD_H_
#define _VIDEOTHREAD_H_

#include "Thread.h"

class VideoThread:public CThread
{
public:
	VideoThread(class CamaraModule* pCamera);
	~VideoThread();

private:
	class CamaraModule* m_pCamera;
	volatile int m_camId;

public:
	void Run(); //线程入口

public:
	
	void SetCamId(int id);
};

#endif