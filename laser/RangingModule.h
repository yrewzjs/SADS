#ifndef _RANGINGMODULE_H_
#define _RANGINGMODULE_H_
/**
* 该类是测距模块数据采集的主要工作区域，主要起中心调度的作用。
* @author zjs
* @Time 2017
*/
#include <map>

class RangingModule
{
public:
	RangingModule();
	~RangingModule();

private:

	class CalculateRanging*  m_pCalculate;

	class RecvListenThread*  m_pRecvListen;

	class SensorManager*  m_pSensorManager;
public:

	bool InitRangingModule();

	void UninitRangingModule();

	void RangingModuleCaliEntry(class LaserRequest* const request, class LaserResponse* const response);

	void RangingModuleAdjustEntry(class LaserRequest* const request, class LaserResponse* const response);
private:
	//单个请求数据并存入到Calculate里面,未使用到这种方式存数据
	void RequestSensorData(const int sId);  
	//单个请求数据但不存入到Calculate里面,未使用到这种方式存数据
	void RequestSensorData(const int sId, double & data);
	//ip组式采集数据并存入到Calculate里面,使用到这种方式存数据
	void RequestGroupSensorData(const enum SegmentAD segment, CString &failedId);
	//轮询式采集数据并存入到Calculate里面,未采用这种方式采数据
	void RequestGroupSensorDataPoling(const enum SegmentAD segment, CString &failedId);
};

#endif