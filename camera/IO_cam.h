#ifndef IO_CAM_H_
#define IO_CAM_H_

#include <coords_struct.h>
#include <CamIP_manage.h>
#include <WinSock2.h>
#include <Windows.h>
#include<value2transit_formchange.h>
#pragma comment(lib, "ws2_32.lib")

class IO_cam
{
public:
	IO_cam();
	~IO_cam();

private:
	SOCKET cam_socket[n_qianban];
	CamIP cam_ip;
	bool isconnect[n_qianban];
	bool is_recvcoords[n_qianban];//相机是否拍到坐标
	//bool isformat[n_qianban];//坐标格式对不对
	bool is_recv[n_qianban];//recv是否成功
	coords_struct IO_CS;
public:
	void Setpara(CamIP &c_CamIP);//Setpara(CamIPg.get_cam()),赋值cam_ip

	connect_transit Connect();//接收要连接的相机组的相机号，给cam_socket，cam_ip赋值，然后连接

	void DisConnect();

	void Close();

	pic_transit get_pic4show(int n_cam);

	coords_transit Send_RecvData(int group);//接收一次相机传来的坐标至IO_CS

	coords_struct output_CS();//输出坐标


	CString GetIp();


	int GetPort();

	SOCKET GetSocket();


};

#endif