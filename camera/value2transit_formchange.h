#ifndef VALUE2TRANSIT_FORMCHANGE_H
#define VALUE2TRANSIT_FORMCHANGE_H

#include <atlimage.h>
#include<core/core.hpp>  
#include<highgui/highgui.hpp> 
#include <opencv2/opencv.hpp>
#include <coords_struct.h>

struct connect_transit//返回连接信息
{
	bool isallconnect;
	bool which_NOT_connect[n_qianban];//哪一位返回1则哪号相机有问题
};
struct coords_transit//返回发送命令接收坐标的成功与否信息
{
	bool isall_ok;
	//int which_NOT_connect[n_qianban];

	bool which_NOT_recvcoords[n_qianban];
	//int which_NOT_format[n_qianban];

	bool which_NOT_recv[n_qianban];
};
struct pic_transit//返回发送命令接收图像成功与否以及需要显示的图像信息
{
	bool isselected;
	bool isok;
	CImage* pic4show;
};

struct command_transit//返回油缸控制有效与否及具体控制信息，需要显示的坐标信息
{
	//输出油缸控制命令，
	int Cylinder;               //X=1,滑动 ；X=2,旋转；X=3,横摇。X=0则不需要移动
	int dir;					//Y=2,伸 ;Y=,缩 ;Y=0,不动。
	//XY:00,不动。
	bool isfunction;			//传输的坐标结构是否完整，不完整continue
	coords_struct CS4show;
	bool issafe;                //传输的坐标是否安全，不安全return false
};
struct standardize_transit//返回标定成功与否以及需要显示的坐标信息
{
	coords_struct CS4show;
	bool isSTfin;
};


void MatToCImage(cv::Mat &mat, CImage* cImage);

CString output_errnum(bool arr[]);

#endif