//�����������
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

//�������ļ��л�õ�Ҫ�����ip��ؽṹ��
struct CamIP
{
	//�Ƿ��Ǹ�����Ҫ��������
	bool num[n_qianban];

	CString cam_ip[n_qianban];
	int cam_port[n_qianban];

	//�Ƿ���Ҫ����
	bool conn[n_qianban];

	//�����������Ҫ��������Ǳ���
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

	//�������ļ��õ����������IP��ַ��
	bool get_cam();

	//���CamIP
	CamIP out_cam();
};

#endif