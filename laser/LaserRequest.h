#ifndef _LASERREQUEST_H_
#define _LASERREQUEST_H_
/**
* 该类是一次图像模块调用测距模块的请求信息。
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