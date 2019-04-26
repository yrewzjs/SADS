#include "stdafx.h"
#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib, "winmm.lib")  // 声音

#include "AdjustThread.h"
#include "MessageProcess.h"
#include "MxWrapper.h"
#include "SadsDef.h"
#include "MXComonentDef.h"

#include "MxWrapper.h"
#include "CamaraModule.h"
#include "value2transit_formchange.h"

#include "LaserDef.h"
#include "LaserDriver.h"
#include "LaserRequest.h"
#include "LaserResponse.h"
#include "Segments.h"
#include "SADSProject.h"
#include "SADSProjectDlg.h"
#include "DialogCamera.h"
#include "DialogDisplayPartA.h"
#include "DialogDisplayPartB.h"

//静态数据成员初始化
int AdjustThread::D500_Copy = 0;

AdjustThread::AdjustThread()
{
	m_isAdjusting = false;

	m_pSegments = new Segments();
}

AdjustThread::~AdjustThread()
{

	if (m_pSegments != nullptr)
	{
		delete m_pSegments;
		m_pSegments = nullptr;
	}
}

void AdjustThread::InitAdjustThread(MxWrapper* pMxWrapper, LaserDriver* pLaser, CamaraModule* pCamera)
{
	m_pMxWrapper = pMxWrapper;
	m_pLaser = pLaser;
	m_pCamera = pCamera;
}

void AdjustThread::Run()
{
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;

	while (true)
	{
		if (GetExit() == true)
		{
			break;
		}

		//定时5s扫描一次
		Sleep(1000*5);

		// 有请求发来
		short D500_action = 0;
		if (m_pMxWrapper->readBitD500(D500_action) != 0)
		{
			LOG(WARNING) << MessageProcess::ProcessMessage(_T("读取PLC数据D500失败"));
		}

		if (D500_action != 0 || AdjustThread::getD500_Copy() != 0)
		{
			// 读取当前管片
			short D2025_curSeg = 0;
			if (m_pMxWrapper->readBitD2025(D2025_curSeg) != 0)
			{
				LOG(WARNING) << MessageProcess::ProcessMessage(_T("读取PLC数据D2025失败"));
			}
			D2025_curSeg = 6;
			Segment seg = SadsDef::ParseSegment(D2025_curSeg);
			SegmentAD segment = SadsDef::TransformSegment(seg);
			// 显示管片状态
			m_pMainDialog->m_DialogDisplayA->ShowSegmentMsg(m_pSegments->UpdateAndGetSegments(seg, SPLICING));

			if (D500_action == 1 || AdjustThread::getD500_Copy() == 1)  // 抓取
			{
				bool isSucc = AutoCatch(segment);
				AdjustThread::setD500_Copy(0);
				m_isAdjusting = false; // 置为假
				if (isSucc == false)
				{
					m_pMainDialog->m_DialogDisplayA->ShowSegmentMsg(m_pSegments->UpdateAndGetSegments(seg, SPLICE_FAILED));
				}
			}
			else if (D500_action == 2) // 拼装
			{
				bool isSucc = AutoSplice(segment);
				m_isAdjusting = false; // 置为假
				if (isSucc == true)
				{
					m_pMainDialog->m_DialogDisplayA->ShowSegmentMsg(m_pSegments->UpdateAndGetSegments(seg, SPLICE_FIN));
				}
				else
				{
					m_pMainDialog->m_DialogDisplayA->ShowSegmentMsg(m_pSegments->UpdateAndGetSegments(seg, SPLICE_FAILED));
				}
			}
		}
	}

	LOG(INFO) << MessageProcess::ProcessMessage(_T("管片调整线程已退出"));
}

bool AdjustThread::AutoCatch(const SegmentAD segment)
{
	m_isAdjusting = true; // 置为真
	// 下发精定位进行中,写入D501=1；
	short D501_loc = 1;
	if (m_pMxWrapper->writeBitD501(D501_loc) != 0)
	{
		this->dealException(_T("写入D501失败"), 3);
		return false;
	}

	// 第零步、测距第零步：判断管片姿态是否偏差过大
	AdjustStep step;

	/*if (segment == SEGAD_ROOF)
	{
		step = RC_STEP_ZERO;
	}
	else
	{
		step = GC_STEP_ZERO;
	}
	MessageProcess::ProcessMessage(_T("第零步开始"));
	if (this->laserAdjustment(step, segment) == false)
	{
		return false;
	}
	MessageProcess::ProcessMessage(_T("第零步完成"));*/

	// 第一步、测距第一步：调整到相机高度
	if (segment == SEGAD_ROOF)
	{
		step = RC_STEP_FIR;
	}
	else
	{
		step = GC_STEP_FIR;
	}
	MessageProcess::ProcessMessage(_T("第一步开始"));
	if (this->laserAdjustment(step, segment) == false)
	{
		return false;
	}
	MessageProcess::ProcessMessage(_T("第一步完成"));

	// 第二步、图像第一步：对准管片
	MessageProcess::ProcessMessage(_T("第二步开始"));
	if (this->cameraAdjustment(true, segment) == false)
	{
		return false;
	}
	MessageProcess::ProcessMessage(_T("第二步完成"));

	// 第三步、测距第二步：贴合吸住管片
	if (segment == SEGAD_ROOF)
	{
		step = RC_STEP_SEC;
	}
	else
	{
		step = GC_STEP_SEC;
	}
	MessageProcess::ProcessMessage(_T("第三步开始"));
	if (this->laserAdjustment(step, segment) == false)
	{
		return false;
	}
	MessageProcess::ProcessMessage(_T("第三步完成"));

	// 第四步：下发精定位已完成,写入D501=2；
	D501_loc = 2;
	MessageProcess::ProcessMessage(_T("第四步开始"));
	if (m_pMxWrapper->writeBitD501(D501_loc) != 0)
	{
		this->dealException(_T("写入D501失败"), 3);
		return false;
	}
	MessageProcess::ProcessMessage(_T("第四步完成"));
	return true;
}

bool AdjustThread::AutoSplice(SegmentAD segment)
{
	m_isAdjusting = true; // 置为真

	// 下发精定位进行中,写入D501=1；
	short D501_loc = 1;
	if (m_pMxWrapper->writeBitD501(D501_loc) != 0)
	{
		this->dealException(_T("写入D501失败"), 3);
		return false;
	}

	// 第一步、测距第一步：
	AdjustStep step;
	if (segment == SEGAD_ROOF)
	{
		step = R_STEP_FIR;
	}
	else
	{
		step = G_STEP_FIR;
	}

	if (this->laserAdjustment(step, segment) == false)
	{
		return false;
	}

	// 第二步、图像第一步：对准管片
	if (this->cameraAdjustment(false, segment) == false)
	{
		return false;
	}

	// 第三步、测距第二步：贴合吸住管片
	if (segment == SEGAD_ROOF)
	{
		step = R_STEP_SEC;
	}
	else
	{
		step = G_STEP_SEC;
	}

	if (this->laserAdjustment(step, segment) == false)
	{
		return false;
	}

	// 第四步、图像第二步：
	if (this->cameraAdjustment(false, segment) == false)
	{
		return false;
	}

	// 第五步：下发精定位已完成,写入D501=2；
	D501_loc = 2;
	if (m_pMxWrapper->writeBitD501(D501_loc) != 0)
	{
		this->dealException(_T("写入D501失败"), 3);
		return false;
	}
	return true;
}

void AdjustThread::dealException(CString exceptionMsg, int moduleId)
{
	//显示到界面
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	switch (moduleId)
	{
	case 1:
		m_pMainDialog->m_DialogDisplayB->ShowLaserStateMsg(EXCEPTION);
		break;
	case 2:
		m_pMainDialog->m_DialogDisplayB->ShowCameraStateMsg(EXCEPTION);
		break;
	case 3:
		m_pMainDialog->m_DialogDisplayB->ShowPlcStateMsg(EXCEPTION);
		break;
	default:
		break;
	}

	LOG(ERROR) << MessageProcess::ProcessMessage(exceptionMsg);
	//播放声音
	const CString filePath = MessageProcess::GetFilePath(_T("media\\warning1.wav"));
	PlaySound(filePath, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);  //指定文件，异步，重复播放
	::Sleep(1000 * 5);
	//停止播放
	PlaySound(NULL, NULL, SND_FILENAME);
}


bool AdjustThread::laserAdjustment(AdjustStep step, SegmentAD segment)
{
	m_isStopAdjusting = false; // 置假

	//起始命令：控制流程确定具体命令
	LaserRequest pRequest; //请求
	LaserResponse pResponse;

	//设置请求命令
	pRequest.SetAdjustStep(step);
	pRequest.SetSegmentAD(segment);

	while (true)
	{
		if (m_isStopAdjusting)
		{
			break;
		}

		//重置响应
		pResponse.ResetResponse();
		// 调用测距模块
		m_pLaser->CallRangingAdjust(&pRequest, &pResponse);

		// 是否有异常
		if (pResponse.hasException())
		{
			//写上位机异常
			short D514_exc = 2;
			if (m_pMxWrapper->writeBitD514(D514_exc) != 0)
			{
				this->dealException(_T("写入D514失败"), 3);
				return false;
			}
			this->dealException(_T("测距模块异常，请检查"), 1);
			return false;
		}

		// 显示测距数据到界面
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_DialogDisplayB->ShowLaserMsg(pResponse.GetRangingStruct());

		// 下发油缸动作
		CylinderAD cld;
		Direction dire;
		pResponse.AcquireCylinderAction(cld, dire);
		if (m_pMxWrapper->writeCylinderAction(cld, dire) != 0)
		{
			this->dealException(_T("写入PLC油缸动作失败"), 3);
			return false;
		}

		// 是否完成调整
		if (pResponse.GetIsFinished())
		{
			break;
		}

		Sleep(1000);
	}

	return true;
}

bool AdjustThread::cameraAdjustment(bool isCatch, SegmentAD segment)
{
	m_isStopAdjusting = false; // 置假

	int cam_group = 0;
	if (isCatch)
	{
		if (segment == SEGAD_ROOF)
		{
			cam_group = 2; // 封顶块抓取
		}
		else
		{
			cam_group = 1; // 普通块抓取
		}
	}
	else
	{
		switch (segment)
		{
		case SEGAD_GENE_FIR:
			cam_group = 3; // 普通块第一片拼装
			break;
		case SEGAD_GENE_LEFT:
			cam_group = 4; // 普通块左拼装
			break;
		case SEGAD_GENE_RIGHT:
			cam_group = 5; //普通块右拼装
			break;
		case SEGAD_ROOF:
			cam_group = 6; //封顶块拼装 
			break;
		default:
			break;
		}
	}
	m_pCamera->Clearcylinder_r();
	while (true)
	{
		if (m_isStopAdjusting)
		{
			break;
		}

		// 调用图像模块
		command_transit transit = m_pCamera->cylinderCommand(cam_group); // 相当于赋值做了备份

		// 是否有异常
		if (transit.isfunction == false)
		{
			////写上位机异常
			//short D514_exc = 2;
			//if (m_pMxWrapper->writeBitD514(D514_exc) != 0)
			//{
			//	this->dealException(_T("写入D514失败"), 3);
			//	return false;
			//}
			//this->dealException(_T("图像模块异常，请检查"), 2);
			//return false;
			continue;  // 确认无问题，2019.1.13
		}

		if (transit.issafe == false)
		{
			//写上位机异常
			short D514_exc = 2;
			if (m_pMxWrapper->writeBitD514(D514_exc) != 0)
			{
				this->dealException(_T("写入D514失败"), 3);
				return false;
			}
			this->dealException(_T("图像模块异常，请检查"), 2);
			return false;
		}

		// 显示图像数据到界面
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_DialogDisplayB->ShowCameraMsg(transit.CS4show);

		// 是否完成调整
		if (transit.Cylinder == 0 && transit.dir == 0)
		{
			break;
		}

		// 下发油缸动作
		CylinderAD cld;
		Direction dire;
		this->parseCamCldAction(transit.Cylinder, transit.dir, cld, dire);
		if (m_pMxWrapper->writeCylinderAction2(cld, dire) != 0)
		{
			this->dealException(_T("写入PLC油缸动作失败"), 3);
			return false;
		}
	}
	return true;
}


void AdjustThread::parseCamCldAction(int cld, int dire, CylinderAD &clda, Direction &direc)
{
	switch (cld)
	{
	case 1:
		clda = CLD_SLIDE;
		break;
	case 2:
		clda = CLD_ROTATE;
		break;
	case 3:
		clda = CLD_ROLL;
		break;
	}

	switch (dire)
	{
	case 0:
		direc = KEEP;
		break;
	case 1:
		direc = WITHDREW;
		break;
	case 2:
		direc = STRETCH;
		break;
	}
}


void AdjustThread::SetIsAdjusting(bool isAdj)
{
	m_isAdjusting = isAdj;
}

bool AdjustThread::GetIsAdjusting()
{
	return m_isAdjusting;
}

void AdjustThread::StopAdjust()
{
	m_isStopAdjusting = true;
}


void AdjustThread::setD500_Copy(int _num)
{
	D500_Copy = _num;
}

int AdjustThread::getD500_Copy()
{
	return D500_Copy;
}