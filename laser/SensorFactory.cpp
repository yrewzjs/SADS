#include "stdafx.h"
#include "SensorFactory.h"

#include "Sensor.h"
#include "SensorKeyence.h"
#include "SensorSick.h"
#include "LaserDef.h"

SensorFactory::SensorFactory()
{
}

SensorFactory::~SensorFactory()
{
}


Sensor* SensorFactory::GetSensor(SensorBrand type)
{
	switch(type)
	{
	case SICK:
		return new SensorSick();
	case KEYENCE:
		return new SensorKeyence();
	default:
		return new SensorKeyence();
	}
}

SensorFactory* SensorFactory::m_pInstance = nullptr;
SensorFactory* SensorFactory::GetSensorFactory()
{
	if (m_pInstance == nullptr)
		m_pInstance = new SensorFactory();
	return m_pInstance;
}

void SensorFactory::DestroyInstance(){

	if (m_pInstance != nullptr){
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}