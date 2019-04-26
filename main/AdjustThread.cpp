#include "stdafx.h"
#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib, "winmm.lib")  // ����

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

//��̬���ݳ�Ա��ʼ��
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

		//��ʱ5sɨ��һ��
		Sleep(1000*5);

		// ��������
		short D500_action = 0;
		if (m_pMxWrapper->readBitD500(D500_action) != 0)
		{
			LOG(WARNING) << MessageProcess::ProcessMessage(_T("��ȡPLC����D500ʧ��"));
		}

		if (D500_action != 0 || AdjustThread::getD500_Copy() != 0)
		{
			// ��ȡ��ǰ��Ƭ
			short D2025_curSeg = 0;
			if (m_pMxWrapper->readBitD2025(D2025_curSeg) != 0)
			{
				LOG(WARNING) << MessageProcess::ProcessMessage(_T("��ȡPLC����D2025ʧ��"));
			}
			D2025_curSeg = 6;
			Segment seg = SadsDef::ParseSegment(D2025_curSeg);
			SegmentAD segment = SadsDef::TransformSegment(seg);
			// ��ʾ��Ƭ״̬
			m_pMainDialog->m_DialogDisplayA->ShowSegmentMsg(m_pSegments->UpdateAndGetSegments(seg, SPLICING));

			if (D500_action == 1 || AdjustThread::getD500_Copy() == 1)  // ץȡ
			{
				bool isSucc = AutoCatch(segment);
				AdjustThread::setD500_Copy(0);
				m_isAdjusting = false; // ��Ϊ��
				if (isSucc == false)
				{
					m_pMainDialog->m_DialogDisplayA->ShowSegmentMsg(m_pSegments->UpdateAndGetSegments(seg, SPLICE_FAILED));
				}
			}
			else if (D500_action == 2) // ƴװ
			{
				bool isSucc = AutoSplice(segment);
				m_isAdjusting = false; // ��Ϊ��
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

	LOG(INFO) << MessageProcess::ProcessMessage(_T("��Ƭ�����߳����˳�"));
}

bool AdjustThread::AutoCatch(const SegmentAD segment)
{
	m_isAdjusting = true; // ��Ϊ��
	// �·�����λ������,д��D501=1��
	short D501_loc = 1;
	if (m_pMxWrapper->writeBitD501(D501_loc) != 0)
	{
		this->dealException(_T("д��D501ʧ��"), 3);
		return false;
	}

	// ���㲽�������㲽���жϹ�Ƭ��̬�Ƿ�ƫ�����
	AdjustStep step;

	/*if (segment == SEGAD_ROOF)
	{
		step = RC_STEP_ZERO;
	}
	else
	{
		step = GC_STEP_ZERO;
	}
	MessageProcess::ProcessMessage(_T("���㲽��ʼ"));
	if (this->laserAdjustment(step, segment) == false)
	{
		return false;
	}
	MessageProcess::ProcessMessage(_T("���㲽���"));*/

	// ��һ��������һ��������������߶�
	if (segment == SEGAD_ROOF)
	{
		step = RC_STEP_FIR;
	}
	else
	{
		step = GC_STEP_FIR;
	}
	MessageProcess::ProcessMessage(_T("��һ����ʼ"));
	if (this->laserAdjustment(step, segment) == false)
	{
		return false;
	}
	MessageProcess::ProcessMessage(_T("��һ�����"));

	// �ڶ�����ͼ���һ������׼��Ƭ
	MessageProcess::ProcessMessage(_T("�ڶ�����ʼ"));
	if (this->cameraAdjustment(true, segment) == false)
	{
		return false;
	}
	MessageProcess::ProcessMessage(_T("�ڶ������"));

	// �����������ڶ�����������ס��Ƭ
	if (segment == SEGAD_ROOF)
	{
		step = RC_STEP_SEC;
	}
	else
	{
		step = GC_STEP_SEC;
	}
	MessageProcess::ProcessMessage(_T("��������ʼ"));
	if (this->laserAdjustment(step, segment) == false)
	{
		return false;
	}
	MessageProcess::ProcessMessage(_T("���������"));

	// ���Ĳ����·�����λ�����,д��D501=2��
	D501_loc = 2;
	MessageProcess::ProcessMessage(_T("���Ĳ���ʼ"));
	if (m_pMxWrapper->writeBitD501(D501_loc) != 0)
	{
		this->dealException(_T("д��D501ʧ��"), 3);
		return false;
	}
	MessageProcess::ProcessMessage(_T("���Ĳ����"));
	return true;
}

bool AdjustThread::AutoSplice(SegmentAD segment)
{
	m_isAdjusting = true; // ��Ϊ��

	// �·�����λ������,д��D501=1��
	short D501_loc = 1;
	if (m_pMxWrapper->writeBitD501(D501_loc) != 0)
	{
		this->dealException(_T("д��D501ʧ��"), 3);
		return false;
	}

	// ��һ��������һ����
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

	// �ڶ�����ͼ���һ������׼��Ƭ
	if (this->cameraAdjustment(false, segment) == false)
	{
		return false;
	}

	// �����������ڶ�����������ס��Ƭ
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

	// ���Ĳ���ͼ��ڶ�����
	if (this->cameraAdjustment(false, segment) == false)
	{
		return false;
	}

	// ���岽���·�����λ�����,д��D501=2��
	D501_loc = 2;
	if (m_pMxWrapper->writeBitD501(D501_loc) != 0)
	{
		this->dealException(_T("д��D501ʧ��"), 3);
		return false;
	}
	return true;
}

void AdjustThread::dealException(CString exceptionMsg, int moduleId)
{
	//��ʾ������
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
	//��������
	const CString filePath = MessageProcess::GetFilePath(_T("media\\warning1.wav"));
	PlaySound(filePath, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);  //ָ���ļ����첽���ظ�����
	::Sleep(1000 * 5);
	//ֹͣ����
	PlaySound(NULL, NULL, SND_FILENAME);
}


bool AdjustThread::laserAdjustment(AdjustStep step, SegmentAD segment)
{
	m_isStopAdjusting = false; // �ü�

	//��ʼ�����������ȷ����������
	LaserRequest pRequest; //����
	LaserResponse pResponse;

	//������������
	pRequest.SetAdjustStep(step);
	pRequest.SetSegmentAD(segment);

	while (true)
	{
		if (m_isStopAdjusting)
		{
			break;
		}

		//������Ӧ
		pResponse.ResetResponse();
		// ���ò��ģ��
		m_pLaser->CallRangingAdjust(&pRequest, &pResponse);

		// �Ƿ����쳣
		if (pResponse.hasException())
		{
			//д��λ���쳣
			short D514_exc = 2;
			if (m_pMxWrapper->writeBitD514(D514_exc) != 0)
			{
				this->dealException(_T("д��D514ʧ��"), 3);
				return false;
			}
			this->dealException(_T("���ģ���쳣������"), 1);
			return false;
		}

		// ��ʾ������ݵ�����
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_DialogDisplayB->ShowLaserMsg(pResponse.GetRangingStruct());

		// �·��͸׶���
		CylinderAD cld;
		Direction dire;
		pResponse.AcquireCylinderAction(cld, dire);
		if (m_pMxWrapper->writeCylinderAction(cld, dire) != 0)
		{
			this->dealException(_T("д��PLC�͸׶���ʧ��"), 3);
			return false;
		}

		// �Ƿ���ɵ���
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
	m_isStopAdjusting = false; // �ü�

	int cam_group = 0;
	if (isCatch)
	{
		if (segment == SEGAD_ROOF)
		{
			cam_group = 2; // �ⶥ��ץȡ
		}
		else
		{
			cam_group = 1; // ��ͨ��ץȡ
		}
	}
	else
	{
		switch (segment)
		{
		case SEGAD_GENE_FIR:
			cam_group = 3; // ��ͨ���һƬƴװ
			break;
		case SEGAD_GENE_LEFT:
			cam_group = 4; // ��ͨ����ƴװ
			break;
		case SEGAD_GENE_RIGHT:
			cam_group = 5; //��ͨ����ƴװ
			break;
		case SEGAD_ROOF:
			cam_group = 6; //�ⶥ��ƴװ 
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

		// ����ͼ��ģ��
		command_transit transit = m_pCamera->cylinderCommand(cam_group); // �൱�ڸ�ֵ���˱���

		// �Ƿ����쳣
		if (transit.isfunction == false)
		{
			////д��λ���쳣
			//short D514_exc = 2;
			//if (m_pMxWrapper->writeBitD514(D514_exc) != 0)
			//{
			//	this->dealException(_T("д��D514ʧ��"), 3);
			//	return false;
			//}
			//this->dealException(_T("ͼ��ģ���쳣������"), 2);
			//return false;
			continue;  // ȷ�������⣬2019.1.13
		}

		if (transit.issafe == false)
		{
			//д��λ���쳣
			short D514_exc = 2;
			if (m_pMxWrapper->writeBitD514(D514_exc) != 0)
			{
				this->dealException(_T("д��D514ʧ��"), 3);
				return false;
			}
			this->dealException(_T("ͼ��ģ���쳣������"), 2);
			return false;
		}

		// ��ʾͼ�����ݵ�����
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_DialogDisplayB->ShowCameraMsg(transit.CS4show);

		// �Ƿ���ɵ���
		if (transit.Cylinder == 0 && transit.dir == 0)
		{
			break;
		}

		// �·��͸׶���
		CylinderAD cld;
		Direction dire;
		this->parseCamCldAction(transit.Cylinder, transit.dir, cld, dire);
		if (m_pMxWrapper->writeCylinderAction2(cld, dire) != 0)
		{
			this->dealException(_T("д��PLC�͸׶���ʧ��"), 3);
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