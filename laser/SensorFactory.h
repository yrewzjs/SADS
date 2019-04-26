#ifndef _SENSORFACTORY_H_
#define _SENSORFACTORY_H_
/*
	�򵥹���ģʽ����һ���������������������һ��Sensor���ʵ��

*/
class Sensor;
enum SensorBrand;

class SensorFactory
{
private:
	SensorFactory();

public:
	~SensorFactory();

private:
	static SensorFactory* m_pInstance;

public:
	Sensor* GetSensor(SensorBrand type);

	static SensorFactory* GetSensorFactory();

	static void DestroyInstance();
};

#endif
