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
	bool is_recvcoords[n_qianban];//����Ƿ��ĵ�����
	//bool isformat[n_qianban];//�����ʽ�Բ���
	bool is_recv[n_qianban];//recv�Ƿ�ɹ�
	coords_struct IO_CS;
public:
	void Setpara(CamIP &c_CamIP);//Setpara(CamIPg.get_cam()),��ֵcam_ip

	connect_transit Connect();//����Ҫ���ӵ�����������ţ���cam_socket��cam_ip��ֵ��Ȼ������

	void DisConnect();

	void Close();

	pic_transit get_pic4show(int n_cam);

	coords_transit Send_RecvData(int group);//����һ�����������������IO_CS

	coords_struct output_CS();//�������


	CString GetIp();


	int GetPort();

	SOCKET GetSocket();


};

#endif