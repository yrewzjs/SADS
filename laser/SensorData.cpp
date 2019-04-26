#include "stdafx.h"
#include "SensorData.h"

SensorData::SensorData()
{
	m_caliValue = 0;
	m_roofCaliValue = 0;
	m_caliGcHeight = 0;
	m_caliRcHeight = 0;
}

SensorData::~SensorData()
{
}

//
//void SensorData::SetMeasValue(double value){
//	m_measValue = value;
//}
//
//double SensorData::GetMeasValue(){
//	return m_measValue;
//}
//

void SensorData::SetGeneCaliValue(double value)
{
	m_caliValue = value;
}

double SensorData::GetGeneCaliValue()
{
	return m_caliValue;
}


void SensorData::SetRoofCaliValue(double value)
{
	m_roofCaliValue = value;
}

double SensorData::GetRoofCaliValue()
{
	return m_roofCaliValue;
}

//void SensorData::SetRealValue(double value){
//	m_realValue = value;
////}
//double SensorData::GetGeneRealValue(){
//	//(300 - meas) - (300 - cali)
//	return  m_caliValue - m_measValue;
//}
//
//double SensorData::GetRoofRealValue(){
//	//(300 - meas) - (300 - cali)
//	return  m_roofCaliValue - m_measValue;
//}

void SensorData::SetCaliGcHeight(double value){
	m_caliGcHeight = value;
}

double SensorData::GetCaliGcHeight(){
	return m_caliGcHeight;
}

void SensorData::SetCaliRcHeight(double value){
	m_caliRcHeight = value;
}

double SensorData::GetCaliRcHeight(){
	return m_caliRcHeight;
}