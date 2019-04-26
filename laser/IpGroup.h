#ifndef _IPGROUP_H_
#define _IPGROUP_H_
/**
* ��������ip��ַ�Ͷ˿ڽ��з�����������ࡣ
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
	map<int, Sensor*>* m_IpSensorMap; //keyΪuid 

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

	Sensor* GetSensor();  //���ظ�ip����������

	void RequestIpGroupSensor(CString & validData);
};

#endif