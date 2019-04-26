#ifndef _LASERDRIVER_H_
#define _LASERDRIVER_H_
/**
* �����ǲ��ģ��������࣬�Ǳ�¶��ͼ������Ľӿڡ�
* �ⲿ����ͨ����ø���������ȵ���InitLaser()����һ���Գ�ʶ�����ģ�顣
* ͨ������GetLaserResponse()�������һ�β��ģ����������
* ������ɺ�ͨ��UninitLaser()��������ʼ�����ģ�顣
* @author zjs
* @Time 2017
*/
#define LASER_NOT_CONNECT  2;

class LaserDriver
{
private:
	LaserDriver();

	~LaserDriver();

private:
	class RangingModule *m_pRangingModule;

	volatile bool m_isRangingInit;

	static class LaserDriver* m_pInstance;

private:
	bool static LoadWinsockDll();//��ʼ������winsock DLL�汾

	void static UnloadWinsockDll();

public:
	static class LaserDriver* GetInstance();

	static void DestroyInstance();

	bool InitLaser();

	void UninitLaser();

	void SetIsRangingInit(bool bl);

	bool GetIsRangingInit();

	void CallRangingAdjust(class LaserRequest* const request, class LaserResponse* const response);

	void CallRangingCali(class LaserRequest* const request, class LaserResponse* const response);

};

#endif