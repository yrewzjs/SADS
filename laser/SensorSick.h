#ifndef _SENSORSICK_H_
#define _SENSORSICK_H_
#include "Sensor.h"

class SensorSick:public Sensor
{
public:
	SensorSick();
	virtual ~SensorSick();

public:

	virtual void AcquireGroupResponse(CString& validData);

	virtual void AcquireData(double &data);

	virtual void GetAndCheckSensorData(CString _data, int uintId, double &validData);

private:
	void CheckSensorData(CString validSData, double &validData);
};
#endif
