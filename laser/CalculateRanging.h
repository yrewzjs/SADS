#ifndef _CALCULATERANGING_H_
#define _CALCULATERANGING_H_

	/**
	* 该类测距模块对传感器数据进行处理的具体实现类，继承自类 Calculate。
    * 该类具备将传感器数据进行处理，将运算结果封装为LaserResponse作为响应返回的功能。
	* @author zjs
	* @Time 2017
	*/

#include "Calculate.h"
#include "MxWrapper.h"


class LaserRequest;
class LaserResponse;

class CalculateRanging : public Calculate
{
public:
	CalculateRanging();
	~CalculateRanging();
public:
	void CalculateProc(LaserRequest* const request, LaserResponse* const response);

	class MxWrapper* m_pMxWrapper2;

private:
	// 普通抓取 第零步(用以检测管片姿态是否偏移过大)
	void AdjustGeneCatchZero(LaserRequest* const request, LaserResponse* const response);
	// 普通抓取 第一步
	void AdjustGeneCatchFir(LaserRequest* const request, LaserResponse* const response);
	// 普通抓取 第二步
	void AdjustGeneCatchSec(LaserRequest* const request, LaserResponse* const response);
	// 普通拼装 第一步
	void AdjustGeneSpliceFir(LaserRequest* const request, LaserResponse* const response);
	// 普通拼装 第二步
	void AdjustGeneSpliceSec(LaserRequest* const request, LaserResponse* const response);

	// 封顶抓取 第零步(用以检测管片姿态是否偏移过大)
	void AdjustRoofCatchZero(LaserRequest* const request, LaserResponse* const response);
	// 封顶抓取 第一步
	void AdjustRoofCatchFir(LaserRequest* const request, LaserResponse* const response);
	// 封顶抓取 第二步
	void AdjustRoofCatchSec(LaserRequest* const request, LaserResponse* const response);
	// 封顶拼装 第一步
	void AdjustRoofSpliceFir(LaserRequest* const request, LaserResponse* const response);
	// 封顶拼装 第二步
	void AdjustRoofSpliceSec(LaserRequest* const request, LaserResponse* const response);


};

#endif