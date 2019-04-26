#ifndef _SENSORFACTORY_H_
#define _SENSORFACTORY_H_
/*
	简单工厂模式，由一个工厂对象决定创建出哪一种Sensor类的实例

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
