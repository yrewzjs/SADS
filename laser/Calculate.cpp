#include "stdafx.h"
#include <algorithm>
#include <list>

#include "Calculate.h"
#include "LaserResponse.h"
#include "SadsDef.h"
#include "LaserDef.h"
#include "MessageProcess.h"
#include "SensorData.h"
#include "ConfigFileUtill.h"

Calculate::Calculate()
{
	m_pDeltaArr = new double[DELTA_NUMBER];
	m_pMeasArr = new double[SENSOR_NUMBER];
	m_pCaliDataArr = new SensorData*[SENSOR_NUMBER];
}

Calculate::~Calculate()
{
	if (m_pCaliDataArr != nullptr)
	{
		for (int id = 0; id < SENSOR_NUMBER; id++)
		{
			delete m_pCaliDataArr[id];
		}

		delete[] m_pCaliDataArr;
		m_pCaliDataArr = nullptr;
	}

	if (m_pDeltaArr != nullptr)
	{
		delete[] m_pDeltaArr;
		m_pDeltaArr = nullptr;
	}

	if (m_pMeasArr != nullptr)
	{
		delete[] m_pMeasArr;
		m_pMeasArr = nullptr;
	}
}

bool Calculate::InitCalculate()
{
	for (int id = 0; id < SENSOR_NUMBER; id++)
	{
		this->InsertSensorData(id);
	}

	for (int id = 0; id < SENSOR_NUMBER; id++)
	{
		m_pMeasArr[id] = 0;
	}

	for (int id = 0; id < DELTA_NUMBER; id++)
	{
		m_pDeltaArr[id] = 0;
	}

	return ConfigFileUtill::LoadLaserCaliFile(this);
}

SensorData* Calculate::GetSensorData(int id)
{
	return m_pCaliDataArr[id];
}

SensorData* Calculate::InsertSensorData(int id)
{
	SensorData* sd = new SensorData();
	m_pCaliDataArr[id] = sd;
	return sd;
}

void Calculate::SetMeasValue(int id, double data)
{
	m_pMeasArr[id] = data;
}

double Calculate::GetMeasValue(int id)
{
	return m_pMeasArr[id];
}

void Calculate::SetCaliGcHeight(int id, double data)
{
	SensorData* sd = GetSensorData(id);
	sd->SetCaliGcHeight(data);
}

double Calculate::GetCaliGcHeight(int id)
{
	SensorData* sd = GetSensorData(id);
	return sd->GetCaliGcHeight();
}

void Calculate::SetCaliRcHeight(int id, double data)
{
	SensorData* sd = GetSensorData(id);
	sd->SetCaliRcHeight(data);
}

double Calculate::GetCaliRcHeight(int id)
{
	SensorData* sd = GetSensorData(id);
	return sd->GetCaliRcHeight();
}

void Calculate::SetGeneCaliValue(int id, double data)
{
	SensorData* sd = GetSensorData(id);
	sd->SetGeneCaliValue(data);
}

double Calculate::GetGeneCaliValue(int id)
{
	SensorData* sd = GetSensorData(id);
	return sd->GetGeneCaliValue();
}

void Calculate::SetRoofCaliValue(int id, double data)
{
	SensorData* sd = GetSensorData(id);
	sd->SetRoofCaliValue(data);
}

double Calculate::GetRoofCaliValue(int id)
{
	SensorData* sd = GetSensorData(id);
	return sd->GetRoofCaliValue();
}

double Calculate::GetRoofRealValue(int id)
{
	//(300 - meas) - (300 - cali)
	return GetRoofCaliValue(id) - GetMeasValue(id);
}

double Calculate::GetGeneRealValue(int id)
{
	return GetGeneCaliValue(id) - GetMeasValue(id);
}

/************************************************************************/

double Calculate::GetDelta(DeltaMark mark)
{
	return m_pDeltaArr[mark];
}

double Calculate::GetDiffOfDelta(DeltaMark sMark, DeltaMark eMark)
{
	return m_pDeltaArr[sMark] - m_pDeltaArr[eMark];
}

double Calculate::MinDelta(DeltaMark _mark1, DeltaMark _mark2, DeltaMark _mark3)
{
	double data_1 = GetDelta(_mark1);
	double data_2 = GetDelta(_mark2);
	double data_3 = GetDelta(_mark3);

	double min = 0;
	if (data_1 <= data_2){
		min = data_1;
	}
	else{
		min = data_2;
	}

	if (data_3 <= min){
		min = data_3;
	}

	return min;
}

double Calculate::MaxAbsDelta(DeltaMark _mark1, DeltaMark _mark2)
{
	double data_1 = GetDelta(_mark1);
	double data_2 = GetDelta(_mark2);
	
	double max = 0;

	if (abs(data_1) >= abs(data_2))
		max = data_1;
	else
		max = data_2;
	return max;
}

double Calculate::MinAbsDelta(DeltaMark _mark1, DeltaMark _mark2){
	double data_1 = GetDelta(_mark1);
	double data_2 = GetDelta(_mark2);

	double min = 0;

	if (abs(data_1) <= abs(data_2))
		min = data_1;
	else
		min = data_2;
	return min;
}

double Calculate::MaxDelta(DeltaMark _mark1, DeltaMark _mark2, DeltaMark _mark3)
{
	double data_1 = GetDelta(_mark1);
	double data_2 = GetDelta(_mark2);
	double data_3 = GetDelta(_mark3);

	double max = 0;
	if (data_1 <= data_2){
		max = data_2;
	}
	else{
		max = data_1;
	}

	if (data_3 >= max){
		max = data_3;
	}

	return max;
}

double Calculate::MaxAbsDelta(DeltaMark _mark1, DeltaMark _mark2, DeltaMark _mark3)
{
	double data_1 =abs(GetDelta(_mark1));
	double data_2 = abs(GetDelta(_mark2));
	double data_3 = abs(GetDelta(_mark3));

	double max = 0;
	if (data_1 <= data_2){
		max = data_2;
	}
	else{
		max = data_1;
	}

	if (data_3 >= max){
		max = data_3;
	}

	return max;
}

double Calculate::MinValue(double v1, double v2, double v3, double v4){
	double min = 0;
	if (v1 < v2){
		min = v1;
	}
	else{
		min = v2;
	}

	if (v3 < min){
		min = v3;
	}

	if (v4 < min){
		min = v4;
	}
	return min;
}

//以ranging_struct结构形式返回m_pSenDataMap中的数据
ranging_struct Calculate::getRangingStruct(const SegmentAD segment)
{
	ranging_struct sensData;
	for (int id = 0; id < SENSOR_NUMBER; id++)
	{
		sensData.meas_v[id] = GetMeasValue(id);
		if (segment == SEGAD_ROOF)
		{
			sensData.cali_v[id] = GetRoofCaliValue(id);
			sensData.real_v[id] = GetRoofRealValue(id);
		}
		else
		{
			sensData.cali_v[id] = GetGeneCaliValue(id);
			sensData.real_v[id] = GetGeneRealValue(id);
		}
	}
	return sensData;
}

bool Calculate::CheckNeedSensorData(const AdjustStep step)
{
	int SenNum = 0;
	SensorMark* idArr;
	switch (step)
	{
	case ADJUST_NOT_START:
		break;
	case GC_STEP_FIR:
	{
						const int num = 4;
						SensorMark markArr[num] = { A0, C0, D0, E0 }; //此步骤所需传感器编号
						SenNum = num;
						idArr = markArr;
						break;
	}
	case GC_STEP_SEC:
	{
						const int num = 4;
						SensorMark markArr[num] = { A0, C0, D0, E0 }; //此步骤所需传感器编号
						SenNum = num;
						idArr = markArr;
						break;
	}
	case G_STEP_FIR:
	{
					   const int num = 6;
					   SensorMark markArr[num] = { A0, A2, B0, B2, C0, C2 }; //此步骤所需传感器编号
					   SenNum = num;
					   idArr = markArr;
					   break;
	}
	case G_STEP_SEC:
	{
					   const int num = 9;
					   SensorMark markArr[num] = { A0, A1, A2, B0, B1, B2, C0, C1, C2 }; //此步骤所需传感器编号
					   SenNum = num;
					   idArr = markArr;
					   break;
	}
	case RC_STEP_FIR:
	{
						break;
	}
	case RC_STEP_SEC:
	{
						break;
	}
	case R_STEP_FIR:
	{
					   break;
	}
	case R_STEP_SEC:
	{
					   break;
	}
	}

	CString errMsg = _T("");
	bool Flag = true;
	for (int i = 0; i < SenNum; i++)
	{
		int id = *(idArr++);
		double measData = GetMeasValue(id);
		if (measData == SENSOR_REQ_ERROR_DATA)
		{
			Flag = false;
			CString errId = _T("");
			errId.Format(_T("%2d，"), id);
			errMsg += errId;
		}
	}

	if (Flag == false)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("SENSOR ") + errMsg + _T(" 数据：SENSOR_REQ_ERROR_DATA"));
		return false;
	}
	else
	{
		return true;
	}
}

void Calculate::UpdataDeltaArr(const SegmentAD segment)
{
	m_pDeltaArr[A2_A0] = CalculateDelta(A2, A0 ,segment);
	m_pDeltaArr[B2_B0] = CalculateDelta(B2, B0 ,segment);
	m_pDeltaArr[C2_C0] = CalculateDelta(C2, C0 ,segment);
	m_pDeltaArr[A2_A1] = CalculateDelta(A2, A1 ,segment);
	m_pDeltaArr[B2_B1] = CalculateDelta(B2, B1 ,segment);
	m_pDeltaArr[C2_C1] = CalculateDelta(C2, C1 ,segment);

	m_pDeltaArr[D1_D0] = CalculateDelta(D1, D0 ,segment);
	m_pDeltaArr[E1_E0] = CalculateDelta(E1, E0 ,segment);

	m_pDeltaArr[D0_E0] = CalculateDelta(D0, E0 ,segment);
	m_pDeltaArr[C0_A0] = CalculateDelta(C0, A0 ,segment);
	m_pDeltaArr[D0_C0] = CalculateDelta(D0, C0 ,segment);
	m_pDeltaArr[E0_A0] = CalculateDelta(E0, A0 ,segment);

	m_pDeltaArr[F1_F0] = CalculateDelta(F1, F0 ,segment);
	m_pDeltaArr[G1_G0] = CalculateDelta(G1, G0 ,segment);
	m_pDeltaArr[F0_G0] = CalculateDelta(F0, G0, segment);
	m_pDeltaArr[H0_J0] = CalculateDelta(H0, J0, segment);
	m_pDeltaArr[F0_H0] = CalculateDelta(F0, H0, segment);
	m_pDeltaArr[G0_J0] = CalculateDelta(G0, J0, segment);

}

double Calculate::CalculateDelta(int minuendId, int subtrahendId,const SegmentAD segment)
{
	if (segment == SEGAD_ROOF)
	{
		return GetRoofRealValue(minuendId) - GetRoofRealValue(subtrahendId);
	}
	else
	{
		return GetGeneRealValue(minuendId) - GetGeneRealValue(subtrahendId);
	}
}