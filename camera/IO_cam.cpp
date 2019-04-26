#include "stdafx.h"
#include <IO_cam.h>
#include <string>
using namespace std;
IO_cam::IO_cam()
{
	WSADATA _wsaData;
	WSAStartup(MAKEWORD(2, 3), &_wsaData);
	cam_ip = { { 0 }, { 0 }, { 0 }, { 0 }, { { 0 } } };
	IO_CS = { { 0 } };

	for (int i = 0; i < n_qianban; i++)
	{
		cam_socket[i] = { 0 };
		isconnect[i] = false;
		is_recvcoords[i] = false;//����Ƿ��ĵ�����
		//isformat[i] = false;//�����ʽ�Բ���
		is_recv[i] = false;//recv�Ƿ�ɹ�
	}
}
IO_cam::~IO_cam()
{
	Close();
	WSACleanup();

}

void IO_cam::Close()
{
	for (int i = 0; i < n_qianban; i++)
	{
		if (isconnect[i] == 1)
		{
			closesocket(this->cam_socket[i]);
		}
	}
	cam_ip = { { 0 }, { 0 }, { 0 }, { 0 }, { { 0 } } };
	IO_CS = { { 0 } };

	for (int i = 0; i < n_qianban; i++)
	{
		cam_socket[i] = { 0 };
		isconnect[i] = false;
		is_recvcoords[i] = false;//����Ƿ��ĵ�����
		//isformat[i] = false;//�����ʽ�Բ���
		is_recv[i] = false;//recv�Ƿ�ɹ�
	}
}

void IO_cam::Setpara(CamIP &c_CamIP)
{
	cam_ip = c_CamIP;

}

connect_transit IO_cam::Connect()
{
	connect_transit tempCT = { false, { false } };
	using namespace std;
	for (int i = 0; i < n_qianban; i++)
	{
		if (cam_ip.conn[i] == 1)
		{
			string strStl = CT2A(cam_ip.cam_ip[i]);
			const char *ipCh = strStl.c_str();
			cam_socket[i] = socket(AF_INET, SOCK_STREAM, 0);
			if (cam_socket[i] == INVALID_SOCKET)
			{
				isconnect[i] = 0;
			}
			struct sockaddr_in serverAddr;//��ַ�ռ�
			serverAddr.sin_addr.s_addr = inet_addr(ipCh);
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_port = htons(cam_ip.cam_port[i]);
			/*
			��send(),recv()��������ʱ��������״����ԭ�򣬷��ղ���Ԥ�ڽ���,�������շ�ʱ��
			*/
			int nNetTimeout = 3000;//ʱ��3��
			setsockopt(cam_socket[i], SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
			setsockopt(cam_socket[i], SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			//�Է�����ģʽ����connect
			fd_set rfd;      //�������� ��������������Ƿ����
			struct timeval timeout;  //ʱ��ṹ��
			FD_ZERO(&rfd);//�����һ����������
			timeout.tv_sec = 3;//��
			timeout.tv_usec = 0;//һ�����֮һ�룬΢��

			u_long ul = 1;//���������
			ioctlsocket(cam_socket[i], FIONBIO, &ul);//����Ϊ����������
			int sConnect = connect(cam_socket[i], (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
			FD_SET(cam_socket[i], &rfd);
			int ret = select(cam_socket[i], 0, &rfd, 0, &timeout);
			if (ret == 1 && FD_ISSET(cam_socket[i], &rfd))
			{
				isconnect[i] = 1;
				ul = 0;
				ioctlsocket(cam_socket[i], FIONBIO, &ul);
			}
			else
			{
				isconnect[i] = 0;
			}
			/*int sConnect = connect(cam_socket[i], (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
			if (sConnect == SOCKET_ERROR)
			{
			isconnect[i] = 0;
			}
			else
			{
			isconnect[i] = 1;
			}*/
		}
		else isconnect[i] = 0;
	}
	int test = 0;
	for (int i = 0; i < n_qianban; i++)//�ж��Ƿ�����
	{

		if (isconnect[i] == cam_ip.conn[i])
		{
			tempCT.which_NOT_connect[i] = false;
			test = test + 1;
		}
		else
		{
			tempCT.which_NOT_connect[i] = true;
		}
	}
	if (test == n_qianban)//�ж��Ƿ�����
	{
		tempCT.isallconnect = true;
	}
	else
	{
		tempCT.isallconnect = false;
	}
	return tempCT;
}


pic_transit IO_cam::get_pic4show(int n_cam)
{
	pic_transit tempST = {false, false,nullptr};
	if (isconnect[n_cam] == 1)
	{
		tempST.isselected = true;
		send(cam_socket[n_cam], "3", strlen("3")*sizeof(char)+1, 0);
		char temp_coords[480 * 301] = { 0 };
		int irecv = recv(cam_socket[n_cam], temp_coords, 480 * 301, 0);
		//+int leng =strlen(temp_coords);
		if (irecv > 0 && strlen(temp_coords)>480 * 281)
		{
			tempST.isok = true;
			cv::Mat testImage(300, 480, CV_8UC1, temp_coords, 480);

			CImage* image = new CImage();
			tempST.pic4show = image;
			MatToCImage(testImage, tempST.pic4show);
		}
	}
	return tempST;
}

coords_transit IO_cam::Send_RecvData(int group)
{
	coords_transit temp_CT = { false, { false }, { false } };
	IO_CS = { { 0 } };
	IO_CS._group = group;
	for (int i = 0; i < n_qianban; i++)
	{
		cam_ip.num[i] = false;
		is_recvcoords[i] = false;//����Ƿ��ĵ�����
		//isformat[i] = false;//�����ʽ�Բ���
		is_recv[i] = false;//recv�Ƿ�ɹ�
	}

	int test = 0;
	switch (group)//ѡ��С��ֻ����С���ڵ����
	{
	case capngroup:
		cam_ip.num[0] = 1;
		cam_ip.num[1] = 1;
		//cam_ip.num[3] = 1;
		test = 0;
		for (int i = 0; i < n_qianban; i++)//�ж��Ƿ�����
		{

			if (isconnect[i] == cam_ip.conn[i])
			{
				test = test + 1;
			}
		}
		if (test == n_qianban)//�ж��Ƿ�����
		{
			for (int i = 0; i < n_qianban; i++)
			{

				if (cam_ip.num[i] == 1)//�ж��Ƿ�����Ҫ�����
				{
					send(cam_socket[i], "1", strlen("1")*sizeof(char)+1, 0);
				}
			}
			for (int i = 0; i < n_qianban; i++)
			{

				if (cam_ip.num[i] == 1)//�ж��Ƿ�����Ҫ�����
				{
					char temp_coords[MAXBYTE] = { 0 };
					int irecv = recv(cam_socket[i], temp_coords, MAXBYTE, 0);
					if (irecv > 0)
					{
						is_recv[i] = true;
						//�������ֵ
						////////////////////////////////////////////////////////////////
						string s_buff1 = temp_coords;
						int tri_num = -1;//���Ǳ���
						vector<int> test1 = {};
						if (s_buff1[1] == 'a'&& s_buff1[0] == 'a'&& s_buff1[s_buff1.length() - 1] == 'b' &&s_buff1[s_buff1.length() - 2] == 'b' &&s_buff1.length() > 7)
						{

							//vector<int> test1;
							//�������
							//int i = 0;
							int t = 0;
							t = s_buff1.find(",", t);
							while (t != string::npos)
							{
								test1.push_back(t);
								t = s_buff1.find(",", t + 1);
								//i++;

							}
							if (test1.size() == 2)
							{
								tri_num = atoi(s_buff1.substr(2, 1).c_str());
							}
							//else isformat[i] = false;
							if (tri_num == cam_ip.tri_num[i][0])
							{
								IO_CS.c_x[tri_num] = atoi(s_buff1.substr(test1[0] + 1, test1[1] - 1 - test1[0]).c_str());
								IO_CS.c_y[tri_num] = atoi(s_buff1.substr(test1[1] + 1, s_buff1.length() - 3 - test1[1]).c_str());
								//isformat[i] = true;
								is_recvcoords[i] = true;
							}
							else
							{
								//isformat[i] = false;
								is_recvcoords[i] = false;
							}
						}
						/////////////////////////////////////////////////////////
						else is_recvcoords[i] = false;
					}
					else is_recv[i] = false;
				}
			}
		}
		break;

	case capkgroup:

		cam_ip.num[6] = 1;
		cam_ip.num[7] = 1;

		test = 0;
		for (int i = 0; i < n_qianban; i++)//�ж��Ƿ�����
		{

			if (isconnect[i] == cam_ip.conn[i])
			{
				test = test + 1;
			}
		}
		if (test == n_qianban)//�ж��Ƿ�����
		{
			for (int i = 0; i < n_qianban; i++)
			{

				if (cam_ip.num[i] == 1)//�ж��Ƿ�����Ҫ�����
				{
					send(cam_socket[i], "1", strlen("1")*sizeof(char)+1, 0);
				}
			}
			for (int i = 0; i < n_qianban; i++)
			{

				if (cam_ip.num[i] == 1)//�ж��Ƿ�����Ҫ�����
				{
					char temp_coords[MAXBYTE] = { 0 };
					int irecv = recv(cam_socket[i], temp_coords, MAXBYTE, 0);
					if (irecv > 0)
					{
						is_recv[i] = true;
						//�������ֵ
						////////////////////////////////////////////////////////////////
						string s_buff1 = temp_coords;
						int tri_num = -1;//���Ǳ���
						vector<int> test1 = {};
						if (s_buff1[1] == 'a'&& s_buff1[0] == 'a'&& s_buff1[s_buff1.length() - 1] == 'b' &&s_buff1[s_buff1.length() - 2] == 'b' &&s_buff1.length() > 7)
						{

							//vector<int> test1;
							//�������
							//int i = 0;
							int t = 0;
							t = s_buff1.find(",", t);
							while (t != string::npos)
							{
								test1.push_back(t);
								t = s_buff1.find(",", t + 1);
								//i++;

							}
							if (test1.size() == 2)
							{
								tri_num = atoi(s_buff1.substr(2, 1).c_str());
							}
							//else isformat[i] = false;
							if (tri_num == cam_ip.tri_num[i][0])
							{
								IO_CS.c_x[tri_num] = atoi(s_buff1.substr(test1[0] + 1, test1[1] - 1 - test1[0]).c_str());
								IO_CS.c_y[tri_num] = atoi(s_buff1.substr(test1[1] + 1, s_buff1.length() - 3 - test1[1]).c_str());
								//isformat[i] = true;
								is_recvcoords[i] = true;
							}
							else
							{
								//isformat[i] = false;
								is_recvcoords[i] = false;
							}
						}
						/////////////////////////////////////////////////////////
						else is_recvcoords[i] = false;
					}
					else is_recv[i] = false;
				}
			}
		}
		break;

	case assemngroup:

		cam_ip.num[0] = 1;
		cam_ip.num[1] = 1;

		test = 0;
		for (int i = 0; i < n_qianban; i++)//�ж��Ƿ�����
		{

			if (isconnect[i] == cam_ip.conn[i])
			{
				test = test + 1;
			}
		}
		if (test == n_qianban)//�ж��Ƿ�����
		{
			for (int i = 0; i < n_qianban; i++)
			{

				if (cam_ip.num[i] == 1)//�ж��Ƿ�����Ҫ�����
				{
					send(cam_socket[i], "1", strlen("1")*sizeof(char)+1, 0);
				}
			}
			for (int i = 0; i < n_qianban; i++)
			{

				if (cam_ip.num[i] == 1)//�ж��Ƿ�����Ҫ�����
				{
					char temp_coords[MAXBYTE] = { 0 };
					int irecv = recv(cam_socket[i], temp_coords, MAXBYTE, 0);
					if (irecv > 0)
					{
						is_recv[i] = true;
						//�������ֵ
						////////////////////////////////////////////////////////////////
						string s_buff1 = temp_coords;
						int tri_num0 = -1;//���Ǳ���
						int tri_num1 = -1;
						vector<int> test1 = {};
						if (s_buff1[1] == 'a'&& s_buff1[0] == 'a'&& s_buff1[s_buff1.length() - 1] == 'c' &&s_buff1[s_buff1.length() - 2] == 'c' &&s_buff1.length() > 10)
						{

							//vector<int> test1;
							//�������
							//int i = 0;
							int t = 0;
							t = s_buff1.find(",", t);
							while (t != string::npos)
							{
								test1.push_back(t);
								t = s_buff1.find(",", t + 1);
								//i++;

							}
							if (test1.size() == 4)
							{
								tri_num0 = atoi(s_buff1.substr(2, 1).c_str());
								tri_num1 = atoi(s_buff1.substr(test1[2] - 2, 2).c_str());
							}
							//else isformat[i] = false;
							if (tri_num0 == cam_ip.tri_num[i][0] && tri_num1 == cam_ip.tri_num[i][1])
							{
								IO_CS.c_x[tri_num0] = atoi(s_buff1.substr(test1[0] + 1, test1[1] - 1 - test1[0]).c_str());
								IO_CS.c_y[tri_num0] = atoi(s_buff1.substr(test1[1] + 1, test1[2] - 5 - test1[1]).c_str());
								IO_CS.c_x[tri_num1] = atoi(s_buff1.substr(test1[2] + 1, test1[3] - 1 - test1[2]).c_str());
								IO_CS.c_y[tri_num1] = atoi(s_buff1.substr(test1[3] + 1, s_buff1.length() - 3 - test1[3]).c_str());
								//isformat[i] = true;
								is_recvcoords[i] = true;
							}
							else
							{
								//isformat[i] = false;
								is_recvcoords[i] = false;
							}
						}
						/////////////////////////////////////////////////////////
						else is_recvcoords[i] = false;
					}
					else is_recv[i] = false;
				}
			}
		}
		break;

	case assemnlgroup:

		cam_ip.num[0] = 1;
		cam_ip.num[1] = 1;
		cam_ip.num[3] = 1;
		cam_ip.num[5] = 1;

		break;

	case assemnrgroup:

		cam_ip.num[0] = 1;
		cam_ip.num[1] = 1;
		cam_ip.num[2] = 1;
		cam_ip.num[4] = 1;

		break;

	case assemkgroup:

		cam_ip.num[6] = 1;
		cam_ip.num[7] = 1;
		cam_ip.num[8] = 1;
		cam_ip.num[9] = 1;

		break;

	default:
		break;
	}

	int _isok = 0;
	for (int i = 0; i < n_qianban; i++)//�ж�ÿ�������Ƿ����
	{

		if (is_recvcoords[i] == cam_ip.num[i])
		{
			_isok = _isok + 1;
			temp_CT.which_NOT_recvcoords[i] = false;
		}
		else temp_CT.which_NOT_recvcoords[i] = true;
		/*if ( isformat[i] == cam_ip.num[i] )
		{
		_isok = _isok + 1;
		}*/
		if (is_recv[i] == cam_ip.num[i])
		{
			_isok = _isok + 1;
			temp_CT.which_NOT_recv[i] = false;
		}
		else temp_CT.which_NOT_recv[i] = true;
	}
	if (_isok == n_qianban * 2)//�ж�ȫ�������Ƿ���ɣ�����ṹ�Ƿ�������
	{
		temp_CT.isall_ok = true;
		IO_CS.isfunction = true;
	}
	else
	{
		temp_CT.isall_ok = false;
		IO_CS.isfunction = false;
	}
	return temp_CT;
}

coords_struct IO_cam::output_CS()
{

	return IO_CS;

}