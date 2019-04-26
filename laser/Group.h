#ifndef _GROUP_H_
#define _GROUP_H_
/**
* 该类主要功能是将一个功能组的传感器归为一个Group
* 并对这些传感器具有管理的能力。
* @author zjs
* @Time 2017
*/
#include <map>
using namespace std;
class Sensor;
class IpGroup;

class Group
{
public:
	Group();
	~Group();
private:
	map<int, Sensor*>* m_SensorMap; //key是不可以重复的
	map<int, IpGroup*>* m_IpGroupMap; //key  IpGroup  ID 从0递增
	int m_id;
	int m_sensorNum;
	
public:
	void SetId(int _id);
	int GetId();
	void SetSensorNum(int _num);
	int GetSensorNum();

	int GetIpGroupNum();

	IpGroup* GetIpGroup(int ipgroupid);

	map<int, Sensor*>* GetSensorMap();
	
	map<int, IpGroup*>* GetIpGroupMap();

public:
	void pushElement(int id, Sensor* const pSen);
	void pushIpGroup(int id, IpGroup* const pIpGroup);
	Sensor* GetSensor(int id);
};

#endif