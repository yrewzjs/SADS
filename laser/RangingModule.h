#ifndef _RANGINGMODULE_H_
#define _RANGINGMODULE_H_
/**
* �����ǲ��ģ�����ݲɼ�����Ҫ����������Ҫ�����ĵ��ȵ����á�
* @author zjs
* @Time 2017
*/
#include <map>

class RangingModule
{
public:
	RangingModule();
	~RangingModule();

private:

	class CalculateRanging*  m_pCalculate;

	class RecvListenThread*  m_pRecvListen;

	class SensorManager*  m_pSensorManager;
public:

	bool InitRangingModule();

	void UninitRangingModule();

	void RangingModuleCaliEntry(class LaserRequest* const request, class LaserResponse* const response);

	void RangingModuleAdjustEntry(class LaserRequest* const request, class LaserResponse* const response);
private:
	//�����������ݲ����뵽Calculate����,δʹ�õ����ַ�ʽ������
	void RequestSensorData(const int sId);  
	//�����������ݵ������뵽Calculate����,δʹ�õ����ַ�ʽ������
	void RequestSensorData(const int sId, double & data);
	//ip��ʽ�ɼ����ݲ����뵽Calculate����,ʹ�õ����ַ�ʽ������
	void RequestGroupSensorData(const enum SegmentAD segment, CString &failedId);
	//��ѯʽ�ɼ����ݲ����뵽Calculate����,δ�������ַ�ʽ������
	void RequestGroupSensorDataPoling(const enum SegmentAD segment, CString &failedId);
};

#endif