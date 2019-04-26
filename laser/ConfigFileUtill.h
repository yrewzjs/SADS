#ifndef _CONFIGFILEUTILL_H_
#define _CONFIGFILEUTILL_H_

class ConfigFileUtill
{
public:

	static bool LoadLaserConfigFile(class SensorManager* pSenManager); // 解析模块配置文件

	static bool LoadLaserCaliFile(class Calculate* pCalculate);   // 解析模块标定文件

	static bool WriteToCaliFile(int id, double attr, CString sTitle); // 写入模块标定文件

};

#endif