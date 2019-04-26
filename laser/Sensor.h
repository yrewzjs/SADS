#ifndef _SENSOR_H_
#define _SENSOR_H_
/**
* �����ǶԴ�������Ϣ�ķ�װ��
* �����ǶԴ�������Ϣ�ķ�װ��
* @author zjs
* @Time 2017
*/

class IOSocket;
class SyncCondition;
enum SensorBrand;

class Sensor
{
public:
	Sensor();
	virtual ~Sensor();

protected:
	SensorBrand m_brand;
	int m_id;   //��00��ʼ

	CString m_response;
	CString m_request;
	
	CString m_ip;
	int m_port;
	IOSocket* m_IoSocket;
	SyncCondition*  m_pRecvCond;
	bool m_isRequesting;

public:
	void SetBrand(SensorBrand _brand);
	SensorBrand GetBrand();

	void SetId(int id);
	int GetId();

	void SetResponse(CString res);
	CString GetResponse();

	void SetRequest(CString req);
	CString GetRequest();

	void SetIp(CString _ip);
	CString GetIp();

	void SetPort(int _port);
	int GetPort();

	void SetIoSocket(IOSocket* const socket);
	IOSocket* GetIoSocket();
	void InitIoSocket(IOSocket* const socket);

	void SetIsRequesting(bool _bool);
	bool GetIsRequesting();

public:
	bool GetIsConnect();
	void SetIsConnect(bool _connect);

	SyncCondition* GetRecvCondition();

	int Connect(); //����������Ŀ�ģ�1���������ڲ�ʵ�ֵ�ϸ��  2�������ⲿͷ�ļ�����

	void BreakConnect();

	SOCKET GetSocket();

	WSAEVENT GetWsaEvent();

	void ReceivedSignal();

protected:
	CString RequestSensor();

public:
	virtual bool TriggerLaser(){ return true; };

	virtual bool CloseLaser(){ return true; };

	virtual bool GetLaserState(){ return true; };

	virtual int GetUnitId(){ return 0; };

	virtual void SetUnitId(int _gid){  };

	virtual bool GetIsVice(){ return false; };

	virtual void SetIsVice(bool _share){  };

	virtual void AcquireGroupResponse(CString& validData) = 0;

	virtual void AcquireData(double &data) = 0;

	virtual void GetAndCheckSensorData(CString _data, int uintId, double &validData)=0;
	//virtual bool CheckSensorData(CString _data, double &validData) = 0;
};

#endif