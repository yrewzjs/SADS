//管理相机分组
#ifndef CAMIP_MANAGE_H_
#define CAMIP_MANAGE_H_

#include <string>
#include "tinystr.h"   
#include "tinyxml.h"
#include <coords_struct.h>


#define allgroup 0
#define capngroup 1
#define capkgroup 2
#define assemngroup 3
#define assemnlgroup 4
#define assemnrgroup 5
#define assemkgroup 6

//从配置文件中获得的要传输的ip相关结构体
struct CamIP
{
	//是否是该组需要拍摄的相机
	bool num[n_qianban];

	CString cam_ip[n_qianban];
	int cam_port[n_qianban];

	//是否需要连接
	bool conn[n_qianban];

	//该相机可能需要拍摄的三角标编号
	int tri_num[n_qianban][2];
};

bool readcamID(CamIP &myCamIP);
CString GetExePath(BOOL bIncludeSep);
CString GetFilePath(LPCTSTR const strCurFileName);

class CamIPg
{
public:
	CamIPg();
	~CamIPg();
private:
	CamIP c_CamIP;
public:

	//从配置文件得到所有相机的IP地址等
	bool get_cam();

	//输出CamIP
	CamIP out_cam();
};

#endif