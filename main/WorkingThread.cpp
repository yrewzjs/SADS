#include "stdafx.h"
#include <windows.h>

#include "WorkingThread.h"
#include "MxWrapper.h"
#include "CamaraModule.h"

#include "LaserDriver.h"
#include "LaserDef.h"
#include "LaserRequest.h"
#include "LaserResponse.h"
#include "SadsDef.h"

#include "value2transit_formchange.h"
#include "VideoThread.h"
#include "PlcMonitorThread.h"
#include "AdjustThread.h"
#include "MessageProcess.h"

#include "SADSProject.h"
#include "SADSProjectDlg.h"
#include "DialogCamera.h"
#include "DialogDisplayPartA.h"
#include "DialogDisplayPartB.h"
#include "Segments.h"

WorkingThread::WorkingThread()
{
	m_pSyncQueue = new SyncQueue<transit_struct>();
	m_isOpen = false;

	m_pCamera = new CamaraModule();
	m_pVideoThread = new VideoThread(m_pCamera);

	m_pLaser = LaserDriver::GetInstance();

	m_pMxWrapper = new MxWrapper();

	m_pPlcMonitorThread = new PlcMonitorThread(m_pMxWrapper);

	m_pAdjustThread = new AdjustThread();
	// 初始化AdjustThread
	m_pAdjustThread->InitAdjustThread(m_pMxWrapper, m_pLaser, m_pCamera);

}

WorkingThread::~WorkingThread()
{
	if (m_pSyncQueue != nullptr)
	{
		m_pSyncQueue->ClearAll();
		delete m_pSyncQueue;
		m_pSyncQueue = nullptr;
	}

	if (m_pMxWrapper != nullptr)
	{
		delete m_pMxWrapper;
		m_pMxWrapper = nullptr;
	}

	if (m_pVideoThread != nullptr)
	{
		delete m_pVideoThread;
		m_pVideoThread = nullptr;
	}

	if (m_pAdjustThread != nullptr)
	{
		delete m_pAdjustThread;
		m_pAdjustThread = nullptr;
	}

	if (m_pPlcMonitorThread != nullptr)
	{
		delete m_pPlcMonitorThread;
		m_pPlcMonitorThread = nullptr;
	}

	if (m_pCamera != nullptr)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	m_pLaser->DestroyInstance();
}

void WorkingThread::Run()
{
	transit_struct* transit = nullptr;

	while (true)
	{
		if (GetExit() == true)
		{
			break;
		}

		// 获得对话框发出的命令
		if (transit != nullptr)
		{
			delete transit; // 回收内存
		}
		transit = m_pSyncQueue->PopFrontEv();

		//解析命令，进行对应的操作
		int type = transit->type;
		int funcId = transit->funcId;

		if (m_isOpen == false)
		{
			if (type != 0) // 排除系统命令
			{
				LOG(INFO) << MessageProcess::ProcessMessage(_T("系统未开机，该操作无效"));
				continue; //不再向下执行
			}
		}

		if (m_pAdjustThread->GetIsAdjusting() == true) // 调整期间不准下发任何命令，包括系统命令
		{
			if (type != 0 && funcId != 2) // 排除停止拼装命令
			{
				LOG(INFO) << MessageProcess::ProcessMessage(_T("系统正调整管片，该操作无效"));
				continue; //不再向下执行
			}
		}

		switch (type)
		{
		case 0: // 系统
			switch (funcId)
			{
			case 0:
			{
					  w2d_open();  // 开机
					  break;
			}
			case 1:
			{
					  w2d_close();  // 关机
					  break;
			}
			case 2:
			{
					  w2d_stopAuto();  // 停止自动拼装
					  break;
			}
			case 3:
			{
					  w2d_exit();  // 退出
					  break;
			}
			}
			break;
		case 1:  //测距
			switch (funcId)
			{
			case 0:
			{
					  int caliId = transit->value;
					  w2d_caliLaser(caliId); // 标定激光

					  break;
			}
			}
			break;
		case 2: //图像
			switch (funcId)
			{
			case 0:
			{
					  int caliId = transit->value;
					  w2d_caliCamera(caliId); //标定相机
					  break;
			}
			case 1:
			{
					  int camId = transit->value;
					  w2d_takePhoto(camId);  // 拍一张
					  break;
			}
			case 2:
			{
					  int camId = transit->value;
					  w2d_openVideo(camId);  // 视频模式
					  break;
			}
			case 3:
			{
					  w2d_closeVideo();    // 关闭视频
					  break;
			}
			}
			break;
		case 3: // PLC
			switch (funcId)
			{
			case 0:
			{
					  wplc_struct* wplc = (wplc_struct*)transit->pointer;
					  w2d_caliPlc(wplc);  //标定PLC
					  break;
			}
			case 1:
			{
					  wplc_struct* wplc = (wplc_struct*)transit->pointer;
					  w2d_inputPlc(wplc); //PLC输入
					  break;
			}
			}
			break;
		}
	}

	LOG(INFO) << MessageProcess::ProcessMessage(_T("工作线程已退出"));
}

MxWrapper* WorkingThread::getMxWrapper()
{
	return m_pMxWrapper;
}
/*****************  系统  *************/

/*****
	transit_struct transit;
	transit.type = 0;
	transit.funcId = 1;
	transit.pointer = nullptr;
	上述方式在栈上申请内存，把指向栈上的指针存入队列，
	进行线程切换后，这个函数被释放，再去读取该位置的数值，必然产生错误！
	************/

void WorkingThread::d2w_open()
{
	if (m_isOpen == true)
	{
		return;
	}
	else
	{
		/*   初始化PLC，不能在另一个线程中开机，未进行线程初始化，会出错
		但是若未连接PLC，则此方法会阻塞较长时间，导致界面“假死” */
		const long logicNum = 0;
		int pCode = m_pMxWrapper->initActUtl(logicNum);
		if (pCode != 0)
		{
			LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC初始化失败，系统开机失败，请检查线路连接"));
			return;
		}
	}

	transit_struct* transit = new transit_struct;
	transit->type = 0;
	transit->funcId = 0;
	transit->pointer = nullptr;
	transit->value = 0;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}

void WorkingThread::d2w_close()
{
	////反初始化PLC，不能在另一个线程中开机，未进行线程初始化，会出错
	if (m_isOpen == true)
	{
		int pCode = m_pMxWrapper->uninitActUtl();
		if (pCode != 0)
		{
			LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC未能正常反初始化，请检查"));
		}
	}
	else
	{
		return;
	}

	transit_struct* transit = new transit_struct;
	transit->type = 0;
	transit->funcId = 1;
	transit->pointer = nullptr;
	transit->value = 0;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}

void WorkingThread::d2w_stopAuto()
{
	transit_struct* transit = new transit_struct;
	transit->type = 0;
	transit->funcId = 2;
	transit->pointer = nullptr;
	transit->value = 0;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}


void WorkingThread::d2w_exit()
{
	transit_struct* transit = new transit_struct;
	transit->type = 0;
	transit->funcId = 3;
	transit->pointer = nullptr;
	transit->value = 0;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}


void WorkingThread::w2d_open()
{
	////初始化PLC，典型的线程内没有初始化，会失败

	//初始化Laser 
	bool lCode = m_pLaser->InitLaser();
	if (lCode == true)
	{
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_DialogDisplayB->ShowLaserStateMsg(RUNNING);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("测距模块初始化失败，系统开机失败"));
		return;
	}

	//初始化Camera 
	struct connect_transit cam_result = m_pCamera->InitCamaraModule();
	if (cam_result.isallconnect == true)
	{
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_DialogDisplayB->ShowCameraStateMsg(RUNNING);
	}
	else
	{
		CString errMsg = output_errnum(cam_result.which_NOT_connect);
		errMsg = _T("图像模块相机 ") + errMsg + _T("未连接，");
		LOG(INFO) << MessageProcess::ProcessMessage(errMsg + _T("系统开机失败"));
		return;
	}

	// 三大模块均已成功初始化，系统开机成功
	this->m_isOpen = true;
	LOG(INFO) << MessageProcess::ProcessMessage(_T("系统成功开机"));

	// 开启PLC监控线程
	m_pPlcMonitorThread->SetExit(false);
	m_pPlcMonitorThread->Start();

	// 开启plc Timer
	m_pAdjustThread->SetExit(false);
	m_pAdjustThread->Start();
}

void WorkingThread::w2d_close()
{
	//关闭adjust线程
	m_pAdjustThread->SetExit(true);
	m_pAdjustThread->join();

	//关闭plc monitor线程
	m_pPlcMonitorThread->SetExit(true);
	m_pPlcMonitorThread->join();

	//反初始化Laser 
	m_pLaser->UninitLaser();

	//反初始化Camera 
	m_pCamera->UninitCamaraModule();

	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogDisplayB->ShowLaserStateMsg(UNSTART);
	m_pMainDialog->m_DialogDisplayB->ShowCameraStateMsg(UNSTART);
	m_pMainDialog->m_DialogDisplayB->ShowPlcStateMsg(UNSTART);

	// 三大模块均反初始化，系统关机成功
	this->m_isOpen = false;
	LOG(INFO) << MessageProcess::ProcessMessage(_T("系统已经关机"));
}

void WorkingThread::w2d_stopAuto()
{
	if (m_pAdjustThread->GetIsAdjusting() == true) 
	{
		m_pAdjustThread->StopAdjust();
	}
}

void WorkingThread::w2d_exit()
{
	this->w2d_close();

	this->SetExit(true);
}
/*****************  测距  *************/

void WorkingThread::d2w_caliLaser(int caliId)
{
	transit_struct* transit = new transit_struct;
	transit->type = 1;
	transit->funcId = 0;
	transit->pointer = nullptr;
	transit->value = caliId;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}

void WorkingThread::w2d_caliLaser(int caliId)
{
	LaserRequest request;
	LaserResponse response;
	switch (caliId)
	{
	case 0: //CALI_GENE_CATCH
		request.SetCaliStep(CALI_GENE_CATCH);
		break;
	case 1: //CALI_GENE_SPLICE
		request.SetCaliStep(CALI_GENE_SPLICE);
		break;
	case 2: //CALI_GENE_HEIGHT
		request.SetCaliStep(CALI_GENE_HEIGHT);
		break;
	case 3: //CALI_ROOF_CATCH
		request.SetCaliStep(CALI_ROOF_CATCH);
		break;
	case 4: //CALI_ROOF_SPLICE
		request.SetCaliStep(CALI_ROOF_SPLICE);
		break;
	case 5: //CALI_ROOF_HEIGHT
		request.SetCaliStep(CALI_ROOF_HEIGHT);
		break;
	default:
		break;
	}

	//调用标定
	m_pLaser->CallRangingCali(&request, &response);

	// 显示测距数据到界面
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogDisplayB->ShowLaserMsg(response.GetRangingStruct());
}

/*****************  图像  *************/

void WorkingThread::d2w_caliCamera(int caliId)
{
	transit_struct* transit = new transit_struct;
	transit->type = 2;
	transit->funcId = 0;
	transit->pointer = nullptr;
	transit->value = caliId;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}

void  WorkingThread::d2w_takePhoto(int camId)
{
	transit_struct* transit = new transit_struct;
	transit->type = 2;
	transit->funcId = 1;
	transit->pointer = nullptr;
	transit->value = camId;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}

void  WorkingThread::d2w_openVideo(int camId)
{
	transit_struct* transit = new transit_struct;
	transit->type = 2;
	transit->funcId = 2;
	transit->pointer = nullptr;
	transit->value = camId;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}

void  WorkingThread::d2w_closeVideo()
{
	transit_struct* transit = new transit_struct;
	transit->type = 2;
	transit->funcId = 3;
	transit->pointer = nullptr;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}


void WorkingThread::w2d_caliCamera(int caliId)
{
	int group = caliId + 1;
	int photoCount = 10;
	standardize_transit transit = m_pCamera->CamaraStandardize(group, photoCount);

	if (transit.isSTfin == false)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("图像模块标定失败"));
		return;
	}

	//显示到界面
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogDisplayB->ShowCameraMsg(transit.CS4show);
}

void WorkingThread::w2d_takePhoto(int camId)
{
	pic_transit pic = m_pCamera->getpic(camId);

	if (pic.isselected == false)
	{
		CString str = _T("");
		str.Format(_T("相机%2d未连接，不能拍照"), camId);
		LOG(ERROR) << MessageProcess::ProcessMessage(str);
		return;
	}

	if (pic.isok == false)
	{
		CString str = _T("");
		str.Format(_T("相机%2d图片未接收，拍照失败"), camId);
		LOG(ERROR) << MessageProcess::ProcessMessage(str);
		return;
	}

	//显示到界面
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogCamera->ShowImage(pic.pic4show);
	// 释放
	if (pic.pic4show != nullptr)
	{
		delete pic.pic4show;
	}
}

void WorkingThread::w2d_openVideo(int camId)
{
	m_pVideoThread->SetCamId(camId);

	m_pVideoThread->SetExit(false);

	m_pVideoThread->Start();
}

void WorkingThread::w2d_closeVideo()
{
	m_pVideoThread->SetExit(true);
}

/*****************  PLC  *************/

void WorkingThread::d2w_caliPlc(wplc_struct* wplc)
{
	transit_struct* transit = new transit_struct;
	transit->type = 3;
	transit->funcId = 0;
	transit->pointer = wplc;  // wplc_struct* wplc == (wplc_struct*)pointer
	transit->value = 0;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}

void WorkingThread::d2w_inputPlc(wplc_struct* wplc)
{
	transit_struct* transit = new transit_struct;
	transit->type = 3;
	transit->funcId = 1;
	transit->pointer = wplc;  // wplc_struct* wplc == (wplc_struct*)pointer
	transit->value = 0;
	// 压入命令队列
	m_pSyncQueue->PushBackEv(transit);
}


void WorkingThread::w2d_caliPlc(wplc_struct* wplc)
{
	if (wplc == nullptr)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("PLC标定命令为null"));
		return;
	}

	int wCode = m_pMxWrapper->write2Plc(wplc);
	if (wCode != 0)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("PLC标定命令写入失败"));
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC标定命令写入成功"));
	}

	delete wplc; //回收
}

void WorkingThread::w2d_inputPlc(wplc_struct* wplc)
{
	if (wplc == nullptr)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("PLC输入命令为null"));
		return;
	}

	int wCode = m_pMxWrapper->write2Plc(wplc);
	if (wCode != 0)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("PLC输入命令写入失败"));
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC输入命令写入成功"));
	}

	delete wplc; //回收
}
