#ifndef _CALCULATE_H_
#define _CALCULATE_H_
/**
* �������Ҫ�����Ǵ洢�ɼ����Ĵ��������ݣ�������������ݽ��д���
* @author zjs
* @Time 2017
*/

class Calculate
{
public:
	Calculate();
	~Calculate();

private:
	double * m_pDeltaArr; // �洢delta  ����ֵ
	double * m_pMeasArr; // �洢measure  ����ֵ
	class SensorData* *m_pCaliDataArr; //����궨����
private:
	double CalculateDelta(int minuendId, int subtrahendId, const enum SegmentAD segment);//�÷������ṩ��UpdataDeltaVector�����ڲ�����	
	class SensorData* GetSensorData(int id);
	class SensorData* InsertSensorData(int id);

protected:
	double GetDelta(enum DeltaMark mark);
	// ÿ�ο�ʼ����ǰ�ȸ���һ���������
	void UpdataDeltaArr(const enum SegmentAD segment);
	// ÿ�ο�ʼ����ǰ�ȼ��һ���������
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

	//��ranging_struct�ṹ��ʽ����m_pSenDataMap�е�����
	struct ranging_struct getRangingStruct(const enum SegmentAD segment);
};

#endif