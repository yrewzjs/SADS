#ifndef _CONFIGFILEUTILL_H_
#define _CONFIGFILEUTILL_H_

class ConfigFileUtill
{
public:

	static bool LoadLaserConfigFile(class SensorManager* pSenManager); // ����ģ�������ļ�

	static bool LoadLaserCaliFile(class Calculate* pCalculate);   // ����ģ��궨�ļ�

	static bool WriteToCaliFile(int id, double attr, CString sTitle); // д��ģ��궨�ļ�

};

#endif