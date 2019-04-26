#ifndef _SENSORKEYENCE_H_
#define _SENSORKEYENCE_H_
#include "Sensor.h"

class SensorKeyence:public Sensor
{
public:
	SensorKeyence();
	SensorKeyence(int _unitId);
	virtual ~SensorKeyence();

private:
	int m_unitId; //´Ó00¿ªÊ¼
	bool m_isVice;
	bool m_laserState;

public:
	
	CString GetUnitIdStr();

//	void SetLaserState(bool bl);
	
public:
	virtual bool GetLaserState();

	virtual int GetUnitId();

	virtual bool GetIsVice();

	virtual bool TriggerLaser();

	virtual bool CloseLaser();

	virtual void SetIsVice(bool _share);

	virtual void SetUnitId(int _gid);

	virtual void AcquireGroupResponse(CString& validData);

	virtual void AcquireData(double &data);

	virtual void GetAndCheckSensorData(CString _data, int uintId, double &validData);

private:
	void AcquireResponse(const CString commend,CString& validData);

	void CheckSensorData(CString validSData, double &validData);
};

#endif