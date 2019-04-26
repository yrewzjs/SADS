#ifndef _MXWRAPPER_H_
#define _MXWRAPPER_H_
/**
* 该类是PLC模块数据读取控件mx的包装器类
* @author zjs
* @Time 2018
*/
#define PLC_NOT_CONNECT     1587

class MxWrapper
{
public:
	MxWrapper();
	~MxWrapper();

private:
	class CActUtlType* m_pActUtl;      //PLC通信API
	class Synchronized* m_pSynchronized;    // 同步

public:

	struct thrust_cylinder_struct getThrustCylinderStruct(struct rplc_struct *rplc);  //以thrust_cylinder_struct结构形式读取PLC中的数据

	struct cylinder_struct getCylinderStruct(struct rplc_struct *rplc);  //以cylinder_struct结构形式读取PLC中的数据

public:
	//初始化控制对象
	void SetActUtlMx(class CActUtlType* pAct);

	long initActUtl(long nLogicNumber);

	long uninitActUtl();

	long readPlc(struct rplc_struct *rplc);

	long write2Plc(const struct wplc_struct *wplc);

public:
	// 为PLC上发抓取拼装命令  0 无请求 1 抓取 2 拼装
	long readBitD500(short &D500);

	// 为PLC上发当前管片
	long readBitD2025(short &D2025);

	// 下发精定位调整状态 0 未进行 1 进行中 2 完成
	long writeBitD501(short D501);

	// 下发精定位未完成 0：正常  1：异常
	long writeBitD511(short D511);

	// 下发上位机状态 1：正常  2：异常
	long writeBitD514(short D514);

	// 下发油缸动作
	long writeCylinderAction(const enum CylinderAD cld, const enum Direction dir);

	long writeCylinderAction2(enum CylinderAD cld, enum Direction dir);
};

#endif