#include "stdafx.h"
#include "CamaraModule.h"
#include <coords_struct.h>
#include <CamIP_manage.h>
#include <read_writeXML.h>
#include <value2transit_formchange.h>
#include <IO_cam.h>
#include  "coordinate_standardize.h"
#include"ctrl_cam.h"
#include "MessageProcess.h"

using namespace std;

CamaraModule::CamaraModule()
{
	m_pCtrl_cam = new Ctrl_cam();
	m_lock = new mutex();
}

CamaraModule::~CamaraModule()
{
	if (m_pCtrl_cam != nullptr){
		delete m_pCtrl_cam;
		m_pCtrl_cam = nullptr;
	}
	if (m_lock != nullptr){
		delete m_lock;
		m_lock = nullptr;
	}
}

connect_transit CamaraModule::InitCamaraModule()
{
#ifdef CAM_NOT_CONNECT
	connect_transit myconnectSTATE;
	myconnectSTATE.isallconnect = true;
#else
	m_pIO_cam = new IO_cam;
	m_pCamIPg = new CamIPg;

	connect_transit myconnectSTATE = { { 0 } };
	bool isreadOK1 = m_pCamIPg->get_cam();
	m_pIO_cam->Setpara(m_pCamIPg->out_cam());//Setpara(CamIPg.out_cam()),读取配置文件赋值cam_ip

	bool isreadOK2 = m_pCtrl_cam->readxml2st();//读取配置文件赋值标定坐标结构体

	if (isreadOK1 == false)
	{
		MessageProcess::ProcessMessage(_T("读取相机IP配置文件出错"));
	}
	else if (isreadOK2 == false)
	{
		MessageProcess::ProcessMessage(_T("读取标定坐标结构体配置文件出错"));
	}
	else if (isreadOK1 == true && isreadOK2 == true)
	{
		myconnectSTATE = m_pIO_cam->Connect();
	}
#endif

	return myconnectSTATE;
}

void CamaraModule::UninitCamaraModule()
{
#ifdef CAM_NOT_CONNECT
	// do nothing
	if (m_pCoordinate_standardize != nullptr){
		m_pCoordinate_standardize = new Coordinate_standardize(1);
		delete m_pCoordinate_standardize;
		m_pCoordinate_standardize = nullptr;
	}
#else
	m_pIO_cam->Close();
	if (m_pIO_cam != nullptr){
		delete m_pIO_cam;
		m_pIO_cam = nullptr;
	}

	if (m_pCamIPg != nullptr){
		delete m_pCamIPg;
		m_pCamIPg = nullptr;
	}

	//未初始化，在未调用CamaraStandardize的情况下，其初始值并不是nullptr
	if (m_pCoordinate_standardize != nullptr){
		m_pCoordinate_standardize = new Coordinate_standardize(1);
		delete m_pCoordinate_standardize;
		m_pCoordinate_standardize = nullptr;
	}
#endif
}

#ifdef CAM_NOT_CONNECT
	#include<stdlib.h>
	#include<time.h>
	#define random(x) (rand()%x)
#endif

standardize_transit CamaraModule::CamaraStandardize(int group, int count)
{
#ifdef CAM_NOT_CONNECT

	m_lock->lock(); // 加锁
	standardize_transit transit;
	m_pCoordinate_standardize = new Coordinate_standardize(count);
	transit.isSTfin = true;
	srand((unsigned int)time(0));
	for (int i = 0; i < cams_count; i++)
	{
		transit.CS4show.c_x[i] = random(100); 
		transit.CS4show.c_y[i] = random(100);
	}
	delete m_pCoordinate_standardize;
	m_pCoordinate_standardize = nullptr;
	m_lock->unlock(); // 解锁
	return transit;
#else
	m_lock->lock(); // 加锁
	standardize_transit outSTT = { { 0 } };
	if (group<1 || group>6)
	{
		MessageProcess::ProcessMessage(_T("没有输入的组号"));
		m_lock->unlock(); // 解锁
		return outSTT;
	}

	m_pCoordinate_standardize = new Coordinate_standardize(count);
	m_pCoordinate_standardize->coords_renew();

	coords_transit* standardize_Ctransit = new coords_transit[count];//返回是标定中传送坐标需打印的错误信息

	for (int i = 0; i < count; i++)
	{
		standardize_Ctransit[i]= m_pIO_cam->Send_RecvData(group);
		
		if (standardize_Ctransit[i].isall_ok == false)
		{
			CString recvERR_report = output_errnum(standardize_Ctransit[i].which_NOT_recv);
			MessageProcess::ProcessMessage(recvERR_report + _T("号相机未收到消息"));
			CString recvcERR_report = output_errnum(standardize_Ctransit[i].which_NOT_recvcoords);
			MessageProcess::ProcessMessage(recvcERR_report + _T("号相机未解析出三角标坐标"));
		}
		
		
		m_pCoordinate_standardize->add(m_pIO_cam->output_CS());//加上output_CS无论有无效
	}	

	if (m_pCoordinate_standardize->isfunctioned() == true)//此步判断是否多次output_CS整体有效
	{
		outSTT.CS4show = m_pCoordinate_standardize->calcu_avar();
		outSTT.isSTfin = m_pCoordinate_standardize->write_avar2xml();
	}
	else
	{
		MessageProcess::ProcessMessage(_T("未接收足够的坐标信息用以标定"));
	}
	delete[] standardize_Ctransit;

	delete m_pCoordinate_standardize;
	m_pCoordinate_standardize = nullptr;
		
	m_lock->unlock(); // 解锁
	return outSTT;
#endif
}

void CamaraModule::Clearcylinder_r()
{
	m_pCtrl_cam->clearcylinder_r();
}

command_transit CamaraModule::cylinderCommand(int group)
{
#ifdef CAM_NOT_CONNECT
	command_transit transit;
	transit.Cylinder = 0;
	transit.dir = 0;
	transit.isfunction = true;
	srand((unsigned int)time(0));
	for (int i = 0; i < 20;i++)
	{
		transit.CS4show.c_x[i] = random(100); 
		transit.CS4show.c_y[i] = random(100);
	}
	return transit;
#else
	m_lock->lock(); // 加锁
	command_transit Command = { 0, 0, 0 ,0 };
	if (group<1 || group>6)
	{
		m_lock->unlock(); // 解锁
		return Command;
	}

	coords_transit ctrl_transit = m_pIO_cam->Send_RecvData(group);//返回是控制中传送坐标需打印的错误信息
	
	if (ctrl_transit.isall_ok == false)
	{
		CString recvERR_report = output_errnum(ctrl_transit.which_NOT_recv);
		MessageProcess::ProcessMessage(recvERR_report + _T("号相机未收到消息"));
		CString recvcERR_report = output_errnum(ctrl_transit.which_NOT_recvcoords);
		MessageProcess::ProcessMessage(recvcERR_report + _T("号相机未解析出三角标坐标"));
	}
	
	coords_struct outCS= m_pIO_cam->output_CS();
	Command = m_pCtrl_cam->move_action(outCS);
	m_lock->unlock(); // 解锁
	return Command;
#endif
}

pic_transit CamaraModule::getpic(int n_cam)
{
#ifdef CAM_NOT_CONNECT
	// do nothing
	pic_transit transit = { true, true, nullptr };
	return transit;
#else
	m_lock->lock(); // 加锁
	pic_transit PIC2display = { false,false,nullptr};
	if (n_cam<0 || n_cam>9)
	{
		MessageProcess::ProcessMessage(_T("没有输入的相机号"));
		m_lock->unlock(); // 解锁
		return PIC2display;
	}

	PIC2display = m_pIO_cam->get_pic4show(n_cam);
	m_lock->unlock(); // 解锁
	return PIC2display;
#endif
}
