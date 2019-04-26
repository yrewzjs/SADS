#ifndef _MXCOMONENTDEF_H_
#define _MXCOMONENTDEF_H_
/**
* plc通信声明
* @author zjs
* @Time 2018
*/

#define DEVICE_D5XX_NUMBER	    20  // D500
#define DEVICE_D2XXX_NUMBER	    26  // D2000
#define DEVICE_NAME_500     _T("D500")  // 上发抓取，拼装指令
#define DEVICE_NAME_2000    _T("D2000")


/***
	PLC内存数据
*/
struct rplc_struct
{
	short lpsData_D5xx[DEVICE_D5XX_NUMBER];   // D5xx寄存器数据
	short lpsData_D2xxx[DEVICE_D2XXX_NUMBER]; // D2xxx寄存器数据
};

/***
	写入PLC内存结构体
*/
struct wplc_struct
{
	bool hasEnabled;       //是否有使能位

	LPCTSTR enabled_szDevice;  // 使能标志位
	short enabled_lpsData;     // 使能寄存器数据
	long  enabled_lSize = 1;  //  使能位数

	LPCTSTR szDevice;  // 标志位
	short lpsData;     // 寄存器数据
	long  lSize = 1;  //  位数 
};
#endif