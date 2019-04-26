#ifndef _CALCULATERANGING_H_
#define _CALCULATERANGING_H_

	/**
	* ������ģ��Դ��������ݽ��д���ľ���ʵ���࣬�̳����� Calculate��
    * ����߱������������ݽ��д�������������װΪLaserResponse��Ϊ��Ӧ���صĹ��ܡ�
	* @author zjs
	* @Time 2017
	*/

#include "Calculate.h"
#include "MxWrapper.h"


class LaserRequest;
class LaserResponse;

class CalculateRanging : public Calculate
{
public:
	CalculateRanging();
	~CalculateRanging();
public:
	void CalculateProc(LaserRequest* const request, LaserResponse* const response);

	class MxWrapper* m_pMxWrapper2;

private:
	// ��ͨץȡ ���㲽(���Լ���Ƭ��̬�Ƿ�ƫ�ƹ���)
	void AdjustGeneCatchZero(LaserRequest* const request, LaserResponse* const response);
	// ��ͨץȡ ��һ��
	void AdjustGeneCatchFir(LaserRequest* const request, LaserResponse* const response);
	// ��ͨץȡ �ڶ���
	void AdjustGeneCatchSec(LaserRequest* const request, LaserResponse* const response);
	// ��ͨƴװ ��һ��
	void AdjustGeneSpliceFir(LaserRequest* const request, LaserResponse* const response);
	// ��ͨƴװ �ڶ���
	void AdjustGeneSpliceSec(LaserRequest* const request, LaserResponse* const response);

	// �ⶥץȡ ���㲽(���Լ���Ƭ��̬�Ƿ�ƫ�ƹ���)
	void AdjustRoofCatchZero(LaserRequest* const request, LaserResponse* const response);
	// �ⶥץȡ ��һ��
	void AdjustRoofCatchFir(LaserRequest* const request, LaserResponse* const response);
	// �ⶥץȡ �ڶ���
	void AdjustRoofCatchSec(LaserRequest* const request, LaserResponse* const response);
	// �ⶥƴװ ��һ��
	void AdjustRoofSpliceFir(LaserRequest* const request, LaserResponse* const response);
	// �ⶥƴװ �ڶ���
	void AdjustRoofSpliceSec(LaserRequest* const request, LaserResponse* const response);


};

#endif