#ifndef _CALCULATE_H_
#define _CALCULATE_H_
/**
* 该类的主要功能是存储采集到的传感器数据，由其子类对数据进行处理。
* @author zjs
* @Time 2017
*/

class Calculate
{
public:
	Calculate();
	~Calculate();

private:
	double * m_pDeltaArr; // 存储delta  运算值
	double * m_pMeasArr; // 存储measure  测量值
	class SensorData* *m_pCaliDataArr; //储存标定数据
private:
	double CalculateDelta(int minuendId, int subtrahendId, const enum SegmentAD segment);//该方法仅提供给UpdataDeltaVector方法内部调用	
	class SensorData* GetSensorData(int id);
	class SensorData* InsertSensorData(int id);

protected:
	double GetDelta(enum DeltaMark mark);
	// 每次开始运算前先更新一遍计算数据
	void UpdataDeltaArr(const enum SegmentAD segment);
	// 每次开始运算前先检查一遍计算数据
	bool CheckNeedSensorData(const enum AdjustStep step);

	double GetDiffOfDelta(enum DeltaMark sMark, enum DeltaMark eMark);

	double MinDelta(enum DeltaMark _mark1, enum DeltaMark _mark2, enum DeltaMark _mark3);

	double MaxDelta(enum DeltaMark _mark1, enum DeltaMark _mark2, enum DeltaMark _mark3);

	double MaxAbsDelta(enum DeltaMark _mark1, enum DeltaMark _mark2, enum DeltaMark _mark3);

	double MaxAbsDelta(enum DeltaMark _mark1, enum DeltaMark _mark2);

	double MinAbsDelta(enum DeltaMark _mark1, enum DeltaMark _mark2);

	double MinValue(double v1, double v2, double v3, double v4);

public:
	void SetMeasValue(int id, double data);
	double GetMeasValue(int id);

	void SetGeneCaliValue(int id, double data);
	double GetGeneCaliValue(int id);

	void SetRoofCaliValue(int id, double data);
	double GetRoofCaliValue(int id);

	void SetCaliGcHeight(int id, double data);
	double GetCaliGcHeight(int id);

	void SetCaliRcHeight(int id, double data);
	double GetCaliRcHeight(int id);

	double GetGeneRealValue(int id);
	double GetRoofRealValue(int id);

	bool InitCalculate();

	//以ranging_struct结构形式返回m_pSenDataMap中的数据
	struct ranging_struct getRangingStruct(const enum SegmentAD segment);
};

#endif