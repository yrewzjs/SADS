#ifndef _WORKINGTHREAD_H_
#define _WORKINGTHREAD_H_

#include <string>
using std::string;
#include "Thread.h"
#include "SyncQueue.h"
#include "MXComonentDef.h"

/***
	type    方法类型
	 0        系统
	 1        测距
	 2        图像
	 3        PLC
	 4        自动拼装
*/
struct transit_struct
{
	int type;   // 方法类型
	int funcId; // 该类型下的第几个方法
	/* 传输数据指针 */
	void*  pointer;  // void *则为“无类型指针”，指向任何类型的数据,后期强转使用 
	int value;  // 存储数值
};


class WorkingThread:public CThread
{
public:
	WorkingThread();
	~WorkingThread();

private:
	class LaserDriver* m_pLaser;   // 测距模块句柄
	class CamaraModule* m_pCamera;  // 测距模块句柄
	class MxWrapper* m_pMxWrapper; // PLC模块通信句柄

	class VideoThread* m_pVideoThread; //视频线程
	class PlcMonitorThread* m_pPlcMonitorThread; // plc监控器线程
	class AdjustThread* m_pAdjustThread;  // 调整线程

	SyncQueue<transit_struct>* m_pSyncQueue;  //命令队列

	volatile bool m_isOpen; // 是否开机成功

public:
	void Run(); //线程入口

	class MxWrapper* getMxWrapper();
private:

public:
	//策略模式应该是一个比较好的解决方案
	// d2w  displayToWork
	/* 
	  type = 0    系统方法
	  funcId    方法名 
	    0      d2w_open
		1      d2w_close
		2      d2w_stopAuto
		3      d2w_exit
	*/
	void d2w_open();   //  开机  
	void d2w_close();  // 关机
	void d2w_stopAuto();  // 停止自动拼装
	void d2w_exit();   // 退出
	/*
		type = 1    测距模块
		funcId    方法名
		0      d2w_caliLaser
	*/
	void d2w_caliLaser(int caliId);  //标定激光
	
	/*
		type = 2    图像模块
		funcId      方法名
		0      d2w_caliCamera
		1      d2w_takePhoto
		2      d2w_openVideo
	*/
	void d2w_caliCamera(int caliId); //标定相机
	void d2w_takePhoto(int camId);  // 拍一张
	void d2w_openVideo(int camId);  // 视频模式
	void d2w_closeVideo();          // 关闭视频
	/*
		type = 3    PLC模块
		funcId      方法名
		0      d2w_caliPlc
		1      d2w_inputPlc
	*/
	void d2w_caliPlc(wplc_struct* wplc);  //标定PLC    wplc申请的内存在clearAll时存在内存泄漏的风险
	void d2w_inputPlc(wplc_struct* wplc); //PLC输入

private:
	/*   系统	*/
	void w2d_open();   //  开机
	void w2d_close();  // 关机
	void w2d_stopAuto();  // 停止自动拼装
	void w2d_exit();   // 退出

	/*   测距模块	*/
	void w2d_caliLaser(int caliId);  //标定激光

	/*   图像模块 	*/
	void w2d_caliCamera(int caliId); //标定相机
	void w2d_takePhoto(int camId);  // 拍一张
	void w2d_openVideo(int camId);  // 视频模式
	void w2d_closeVideo();          // 关闭视频

	/*   PLC模块   */
	void w2d_caliPlc(wplc_struct* wplc);  //标定PLC
	void w2d_inputPlc(wplc_struct* wplc); //PLC输入
};

#endif