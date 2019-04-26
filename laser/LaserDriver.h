#ifndef _LASERDRIVER_H_
#define _LASERDRIVER_H_
/**
* 该类是测距模块的驱动类，是暴露给图像处理方面的接口。
* 外部可以通过获得该类对象，首先调用InitLaser()方法一次以初识化测距模块。
* 通过调用GetLaserResponse()方法获得一次测距模块运算结果。
* 调整完成后通过UninitLaser()方法反初始化测距模块。
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
	bool static LoadWinsockDll();//初始化加载winsock DLL版本

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