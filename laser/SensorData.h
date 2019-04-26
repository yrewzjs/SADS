#ifndef _SENSORDATA_H_
#define _SENSORDATA_H_
/**
* 该类的主要功能是存储传感器标定数据
* @author zjs
* @Time 2017
*/
class SensorData
{
public:
	SensorData();
	~SensorData();

private:
	double m_caliValue;
	double m_roofCaliValue;
	double m_caliGcHeight;  // 普通块抓取相机高度标定值
	double m_caliRcHeight;  // 封顶块抓取相机高度标定值

	//double m_measValue;
	//double m_realValue;
public:
	void SetGeneCaliValue(double value);
	double GetGeneCaliValue();

	void SetRoofCaliValue(double value);
	double GetRoofCaliValue();

	void SetCaliGcHeight(double value);
	double GetCaliGcHeight();

	void SetCaliRcHeight(double value);
	double GetCaliRcHeight();

	//void SetRealValue(double value);
	//double GetGeneRealValue();
	//double GetRoofRealValue();
	//void SetMeasValue(double value);
	//double GetMeasValue();
};

#endif
