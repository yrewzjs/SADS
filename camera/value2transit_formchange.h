#ifndef VALUE2TRANSIT_FORMCHANGE_H
#define VALUE2TRANSIT_FORMCHANGE_H

#include <atlimage.h>
#include<core/core.hpp>  
#include<highgui/highgui.hpp> 
#include <opencv2/opencv.hpp>
#include <coords_struct.h>

struct connect_transit//����������Ϣ
{
	bool isallconnect;
	bool which_NOT_connect[n_qianban];//��һλ����1���ĺ����������
};
struct coords_transit//���ط��������������ĳɹ������Ϣ
{
	bool isall_ok;
	//int which_NOT_connect[n_qianban];

	bool which_NOT_recvcoords[n_qianban];
	//int which_NOT_format[n_qianban];

	bool which_NOT_recv[n_qianban];
};
struct pic_transit//���ط����������ͼ��ɹ�����Լ���Ҫ��ʾ��ͼ����Ϣ
{
	bool isselected;
	bool isok;
	CImage* pic4show;
};

struct command_transit//�����͸׿�����Ч��񼰾��������Ϣ����Ҫ��ʾ��������Ϣ
{
	//����͸׿������
	int Cylinder;               //X=1,���� ��X=2,��ת��X=3,��ҡ��X=0����Ҫ�ƶ�
	int dir;					//Y=2,�� ;Y=,�� ;Y=0,������
	//XY:00,������
	bool isfunction;			//���������ṹ�Ƿ�������������continue
	coords_struct CS4show;
	bool issafe;                //����������Ƿ�ȫ������ȫreturn false
};
struct standardize_transit//���ر궨�ɹ�����Լ���Ҫ��ʾ��������Ϣ
{
	coords_struct CS4show;
	bool isSTfin;
};


void MatToCImage(cv::Mat &mat, CImage* cImage);

CString output_errnum(bool arr[]);

#endif