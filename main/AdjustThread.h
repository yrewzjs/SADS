#ifndef _ADJUSTTHREAD_H_
#define _ADJUSTTHREAD_H_

#include "Thread.h"

class AdjustThread:public CThread
{
public:
	AdjustThread();
	~AdjustThread();

private:
	class MxWrapper* m_pMxWrapper; // PLC模块句柄
	class LaserDriver* m_pLaser;   // 测距模块句柄
	class CamaraModule* m_pCamera;  // 测距模块句柄

	class Segments* m_pSegments;  // 维护管片状态信息
private:
	volatile bool m_isAdjusting;

	volatile bool m_isStopAdjusting; // 是否退出调整
public:
	void Run(); //线程入口

	void InitAdjustThread(class MxWrapper* pMxWrapper,class LaserDriver* pLaser,class CamaraModule* pCamera);
	
	void SetIsAdjusting(bool isAdj);
	bool GetIsAdjusting();

	void StopAdjust();

private:
	bool AutoCatch(const enum SegmentAD segment);

	bool AutoSplice(const enum SegmentAD segment);

	bool laserAdjustment(enum AdjustStep step, enum SegmentAD segment);

	bool cameraAdjustment(bool isCatch, enum SegmentAD segment);

	// moduleId: 1=laser 2=camera 3=plc,否则其他
	void dealException(CString exceptionMsg, int moduleId);

	void parseCamCldAction(int cld, int dire, enum CylinderAD &clda, enum Direction &direc);

public:
	//将D500替代值置1
	static void setD500_Copy(int _num);
	static int getD500_Copy();

private:
	static int D500_Copy;
};

#endif