#ifndef _MXCOMONENTDEF_H_
#define _MXCOMONENTDEF_H_
/**
* plcͨ������
* @author zjs
* @Time 2018
*/

#define DEVICE_D5XX_NUMBER	    20  // D500
#define DEVICE_D2XXX_NUMBER	    26  // D2000
#define DEVICE_NAME_500     _T("D500")  // �Ϸ�ץȡ��ƴװָ��
#define DEVICE_NAME_2000    _T("D2000")


/***
	PLC�ڴ�����
*/
struct rplc_struct
{
	short lpsData_D5xx[DEVICE_D5XX_NUMBER];   // D5xx�Ĵ�������
	short lpsData_D2xxx[DEVICE_D2XXX_NUMBER]; // D2xxx�Ĵ�������
};

/***
	д��PLC�ڴ�ṹ��
*/
struct wplc_struct
{
	bool hasEnabled;       //�Ƿ���ʹ��λ

	LPCTSTR enabled_szDevice;  // ʹ�ܱ�־λ
	short enabled_lpsData;     // ʹ�ܼĴ�������
	long  enabled_lSize = 1;  //  ʹ��λ��

	LPCTSTR szDevice;  // ��־λ
	short lpsData;     // �Ĵ�������
	long  lSize = 1;  //  λ�� 
};
#endif