#ifndef _SENSORMANAGER_H_
#define _SENSORMANAGER_H_
/**
* �����Ǵ����������ࡣ
* @author zjs
* @Time 2017
*/
#include <map>
using namespace std;
class Sensor;
class Group;
class IpGroup;

class SensorManager
{
public:
	SensorManager();
	~SensorManager();

private:
	map<int, Sensor*>* m_SensorMap;   //key  Sensor   id
	map<int, Group*>* m_GroupMap;     //key  Group    ID ��0����

public:

	bool InitSensorManager(); 

	bool CloseAllLaser(CString &failedId);

	bool TriggerAllLaser(CString &failedId);

public:
	int GetSenNum();

	int GetGroupNum();

	Sensor* GetSensor(int id);

	Sensor* GetCurrentSensor(SOCKET _sock);

	map<int, Sensor*>* GetSensorMap();

	Group* GetGroup(int groupid);

	bool GetAllLaserState(CString &closeId);

	bool CheckConnectState(CString &failedId);
	
	bool JudgeUnitId(int port, int uid); //�������ж�uid�Ϸ���ʹ��

	Sensor* GetMainSensor(CString ip, int port);
	
	bool ContainsSensor(int id);

	bool ConnectLinks(CString &failedId);

	void pushSenElement(int id, Sensor* const pSen);

	void pushGrpElement(int id, Group* const pGroup);
};

#endif