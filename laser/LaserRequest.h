#ifndef _LASERREQUEST_H_
#define _LASERREQUEST_H_
/**
* ������һ��ͼ��ģ����ò��ģ���������Ϣ��
* @author zjs
* @Time 2017
*/

class LaserRequest
{
public:
	LaserRequest();
	~LaserRequest();

private:
	enum AdjustStep m_AdjustStep;
	enum CaliStep m_CaliStep;
	enum SegmentAD m_segmentMark;

public:

	void SetAdjustStep(enum AdjustStep step);
	enum AdjustStep GetAdjustStep();

	void SetSegmentAD(enum SegmentAD segMark);
	enum SegmentAD GetSegmentAD();

	void SetCaliStep(enum CaliStep step);
	enum CaliStep GetCaliStep();
};

#endif