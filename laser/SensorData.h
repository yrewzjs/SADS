#ifndef _SENSORDATA_H_
#define _SENSORDATA_H_
/**
* �������Ҫ�����Ǵ洢�������궨����
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
	double m_caliGcHeight;  // ��ͨ��ץȡ����߶ȱ궨ֵ
	double m_caliRcHeight;  // �ⶥ��ץȡ����߶ȱ궨ֵ

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
