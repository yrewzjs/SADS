#ifndef _ADJUSTTHREAD_H_
#define _ADJUSTTHREAD_H_

#include "Thread.h"

class AdjustThread:public CThread
{
public:
	AdjustThread();
	~AdjustThread();

private:
	class MxWrapper* m_pMxWrapper; // PLCģ����
	class LaserDriver* m_pLaser;   // ���ģ����
	class CamaraModule* m_pCamera;  // ���ģ����

	class Segments* m_pSegments;  // ά����Ƭ״̬��Ϣ
private:
	volatile bool m_isAdjusting;

	volatile bool m_isStopAdjusting; // �Ƿ��˳�����
public:
	void Run(); //�߳����

	void InitAdjustThread(class MxWrapper* pMxWrapper,class LaserDriver* pLaser,class CamaraModule* pCamera);
	
	void SetIsAdjusting(bool isAdj);
	bool GetIsAdjusting();

	void StopAdjust();

private:
	bool AutoCatch(const enum SegmentAD segment);

	bool AutoSplice(const enum SegmentAD segment);

	bool laserAdjustment(enum AdjustStep step, enum SegmentAD segment);

	bool cameraAdjustment(bool isCatch, enum SegmentAD segment);

	// moduleId: 1=laser 2=camera 3=plc,��������
	void dealException(CString exceptionMsg, int moduleId);

	void parseCamCldAction(int cld, int dire, enum CylinderAD &clda, enum Direction &direc);

public:
	//��D500���ֵ��1
	static void setD500_Copy(int _num);
	static int getD500_Copy();

private:
	static int D500_Copy;
};

#endif