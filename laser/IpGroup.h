#ifndef _IPGROUP_H_
#define _IPGROUP_H_
/**
* 该类是以ip地址和端口进行分组管理传感器类。
* @author zjs
* @Time 2017
*/
#include <map>
using namespace std;
class Sensor;

class IpGroup
{
public:
	IpGroup();
	~IpGroup();

private:
	CString m_ip;
	int m_port;
	int m_id;

private:
	map<int, Sensor*>* m_IpSensorMap; //key为uid 

public:
	void SetIp(CString _ip);
	CString GetIp();

	void SetPort(int _port);
	int GetPort();

	void SetId(int _id);
	int GetId();

	map<int, Sensor*>* GetIpSensorMap();
	int GetIpSenNum();

public:
	void pushElement(int uid, Sensor* const pSen);

	Sensor* GetSensor(int uid);

	Sensor* GetSensor();  //返回该ip组主传感器

	void RequestIpGroupSensor(CString & validData);
};

#endif