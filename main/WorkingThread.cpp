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
	// ��ʼ��AdjustThread
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

		// ��öԻ��򷢳�������
		if (transit != nullptr)
		{
			delete transit; // �����ڴ�
		}
		transit = m_pSyncQueue->PopFrontEv();

		//����������ж�Ӧ�Ĳ���
		int type = transit->type;
		int funcId = transit->funcId;

		if (m_isOpen == false)
		{
			if (type != 0) // �ų�ϵͳ����
			{
				LOG(INFO) << MessageProcess::ProcessMessage(_T("ϵͳδ�������ò�����Ч"));
				continue; //��������ִ��
			}
		}

		if (m_pAdjustThread->GetIsAdjusting() == true) // �����ڼ䲻׼�·��κ��������ϵͳ����
		{
			if (type != 0 && funcId != 2) // �ų�ֹͣƴװ����
			{
				LOG(INFO) << MessageProcess::ProcessMessage(_T("ϵͳ��������Ƭ���ò�����Ч"));
				continue; //��������ִ��
			}
		}

		switch (type)
		{
		case 0: // ϵͳ
			switch (funcId)
			{
			case 0:
			{
					  w2d_open();  // ����
					  break;
			}
			case 1:
			{
					  w2d_close();  // �ػ�
					  break;
			}
			case 2:
			{
					  w2d_stopAuto();  // ֹͣ�Զ�ƴװ
					  break;
			}
			case 3:
			{
					  w2d_exit();  // �˳�
					  break;
			}
			}
			break;
		case 1:  //���
			switch (funcId)
			{
			case 0:
			{
					  int caliId = transit->value;
					  w2d_caliLaser(caliId); // �궨����

					  break;
			}
			}
			break;
		case 2: //ͼ��
			switch (funcId)
			{
			case 0:
			{
					  int caliId = transit->value;
					  w2d_caliCamera(caliId); //�궨���
					  break;
			}
			case 1:
			{
					  int camId = transit->value;
					  w2d_takePhoto(camId);  // ��һ��
					  break;
			}
			case 2:
			{
					  int camId = transit->value;
					  w2d_openVideo(camId);  // ��Ƶģʽ
					  break;
			}
			case 3:
			{
					  w2d_closeVideo();    // �ر���Ƶ
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
					  w2d_caliPlc(wplc);  //�궨PLC
					  break;
			}
			case 1:
			{
					  wplc_struct* wplc = (wplc_struct*)transit->pointer;
					  w2d_inputPlc(wplc); //PLC����
					  break;
			}
			}
			break;
		}
	}

	LOG(INFO) << MessageProcess::ProcessMessage(_T("�����߳����˳�"));
}

MxWrapper* WorkingThread::getMxWrapper()
{
	return m_pMxWrapper;
}
/*****************  ϵͳ  *************/

/*****
	transit_struct transit;
	transit.type = 0;
	transit.funcId = 1;
	transit.pointer = nullptr;
	������ʽ��ջ�������ڴ棬��ָ��ջ�ϵ�ָ�������У�
	�����߳��л�������������ͷţ���ȥ��ȡ��λ�õ���ֵ����Ȼ��������
	************/

void WorkingThread::d2w_open()
{
	if (m_isOpen == true)
	{
		return;
	}
	else
	{
		/*   ��ʼ��PLC����������һ���߳��п�����δ�����̳߳�ʼ���������
		������δ����PLC����˷����������ϳ�ʱ�䣬���½��桰������ */
		const long logicNum = 0;
		int pCode = m_pMxWrapper->initActUtl(logicNum);
		if (pCode != 0)
		{
			LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC��ʼ��ʧ�ܣ�ϵͳ����ʧ�ܣ�������·����"));
			return;
		}
	}

	transit_struct* transit = new transit_struct;
	transit->type = 0;
	transit->funcId = 0;
	transit->pointer = nullptr;
	transit->value = 0;
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}

void WorkingThread::d2w_close()
{
	////����ʼ��PLC����������һ���߳��п�����δ�����̳߳�ʼ���������
	if (m_isOpen == true)
	{
		int pCode = m_pMxWrapper->uninitActUtl();
		if (pCode != 0)
		{
			LOG(INFO) << MessageProcess::ProcessMessage(_T("PLCδ����������ʼ��������"));
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
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}

void WorkingThread::d2w_stopAuto()
{
	transit_struct* transit = new transit_struct;
	transit->type = 0;
	transit->funcId = 2;
	transit->pointer = nullptr;
	transit->value = 0;
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}


void WorkingThread::d2w_exit()
{
	transit_struct* transit = new transit_struct;
	transit->type = 0;
	transit->funcId = 3;
	transit->pointer = nullptr;
	transit->value = 0;
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}


void WorkingThread::w2d_open()
{
	////��ʼ��PLC�����͵��߳���û�г�ʼ������ʧ��

	//��ʼ��Laser 
	bool lCode = m_pLaser->InitLaser();
	if (lCode == true)
	{
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_DialogDisplayB->ShowLaserStateMsg(RUNNING);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("���ģ���ʼ��ʧ�ܣ�ϵͳ����ʧ��"));
		return;
	}

	//��ʼ��Camera 
	struct connect_transit cam_result = m_pCamera->InitCamaraModule();
	if (cam_result.isallconnect == true)
	{
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_DialogDisplayB->ShowCameraStateMsg(RUNNING);
	}
	else
	{
		CString errMsg = output_errnum(cam_result.which_NOT_connect);
		errMsg = _T("ͼ��ģ����� ") + errMsg + _T("δ���ӣ�");
		LOG(INFO) << MessageProcess::ProcessMessage(errMsg + _T("ϵͳ����ʧ��"));
		return;
	}

	// ����ģ����ѳɹ���ʼ����ϵͳ�����ɹ�
	this->m_isOpen = true;
	LOG(INFO) << MessageProcess::ProcessMessage(_T("ϵͳ�ɹ�����"));

	// ����PLC����߳�
	m_pPlcMonitorThread->SetExit(false);
	m_pPlcMonitorThread->Start();

	// ����plc Timer
	m_pAdjustThread->SetExit(false);
	m_pAdjustThread->Start();
}

void WorkingThread::w2d_close()
{
	//�ر�adjust�߳�
	m_pAdjustThread->SetExit(true);
	m_pAdjustThread->join();

	//�ر�plc monitor�߳�
	m_pPlcMonitorThread->SetExit(true);
	m_pPlcMonitorThread->join();

	//����ʼ��Laser 
	m_pLaser->UninitLaser();

	//����ʼ��Camera 
	m_pCamera->UninitCamaraModule();

	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogDisplayB->ShowLaserStateMsg(UNSTART);
	m_pMainDialog->m_DialogDisplayB->ShowCameraStateMsg(UNSTART);
	m_pMainDialog->m_DialogDisplayB->ShowPlcStateMsg(UNSTART);

	// ����ģ�������ʼ����ϵͳ�ػ��ɹ�
	this->m_isOpen = false;
	LOG(INFO) << MessageProcess::ProcessMessage(_T("ϵͳ�Ѿ��ػ�"));
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
/*****************  ���  *************/

void WorkingThread::d2w_caliLaser(int caliId)
{
	transit_struct* transit = new transit_struct;
	transit->type = 1;
	transit->funcId = 0;
	transit->pointer = nullptr;
	transit->value = caliId;
	// ѹ���������
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

	//���ñ궨
	m_pLaser->CallRangingCali(&request, &response);

	// ��ʾ������ݵ�����
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogDisplayB->ShowLaserMsg(response.GetRangingStruct());
}

/*****************  ͼ��  *************/

void WorkingThread::d2w_caliCamera(int caliId)
{
	transit_struct* transit = new transit_struct;
	transit->type = 2;
	transit->funcId = 0;
	transit->pointer = nullptr;
	transit->value = caliId;
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}

void  WorkingThread::d2w_takePhoto(int camId)
{
	transit_struct* transit = new transit_struct;
	transit->type = 2;
	transit->funcId = 1;
	transit->pointer = nullptr;
	transit->value = camId;
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}

void  WorkingThread::d2w_openVideo(int camId)
{
	transit_struct* transit = new transit_struct;
	transit->type = 2;
	transit->funcId = 2;
	transit->pointer = nullptr;
	transit->value = camId;
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}

void  WorkingThread::d2w_closeVideo()
{
	transit_struct* transit = new transit_struct;
	transit->type = 2;
	transit->funcId = 3;
	transit->pointer = nullptr;
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}


void WorkingThread::w2d_caliCamera(int caliId)
{
	int group = caliId + 1;
	int photoCount = 10;
	standardize_transit transit = m_pCamera->CamaraStandardize(group, photoCount);

	if (transit.isSTfin == false)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("ͼ��ģ��궨ʧ��"));
		return;
	}

	//��ʾ������
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogDisplayB->ShowCameraMsg(transit.CS4show);
}

void WorkingThread::w2d_takePhoto(int camId)
{
	pic_transit pic = m_pCamera->getpic(camId);

	if (pic.isselected == false)
	{
		CString str = _T("");
		str.Format(_T("���%2dδ���ӣ���������"), camId);
		LOG(ERROR) << MessageProcess::ProcessMessage(str);
		return;
	}

	if (pic.isok == false)
	{
		CString str = _T("");
		str.Format(_T("���%2dͼƬδ���գ�����ʧ��"), camId);
		LOG(ERROR) << MessageProcess::ProcessMessage(str);
		return;
	}

	//��ʾ������
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogCamera->ShowImage(pic.pic4show);
	// �ͷ�
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
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}

void WorkingThread::d2w_inputPlc(wplc_struct* wplc)
{
	transit_struct* transit = new transit_struct;
	transit->type = 3;
	transit->funcId = 1;
	transit->pointer = wplc;  // wplc_struct* wplc == (wplc_struct*)pointer
	transit->value = 0;
	// ѹ���������
	m_pSyncQueue->PushBackEv(transit);
}


void WorkingThread::w2d_caliPlc(wplc_struct* wplc)
{
	if (wplc == nullptr)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("PLC�궨����Ϊnull"));
		return;
	}

	int wCode = m_pMxWrapper->write2Plc(wplc);
	if (wCode != 0)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("PLC�궨����д��ʧ��"));
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC�궨����д��ɹ�"));
	}

	delete wplc; //����
}

void WorkingThread::w2d_inputPlc(wplc_struct* wplc)
{
	if (wplc == nullptr)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("PLC��������Ϊnull"));
		return;
	}

	int wCode = m_pMxWrapper->write2Plc(wplc);
	if (wCode != 0)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("PLC��������д��ʧ��"));
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("PLC��������д��ɹ�"));
	}

	delete wplc; //����
}
