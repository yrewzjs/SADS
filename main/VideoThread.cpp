#include "stdafx.h"
#include <windows.h>

#include "VideoThread.h"
#include "CamaraModule.h"

#include "value2transit_formchange.h"
#include "MessageProcess.h"

#include "SADSProject.h"
#include "SADSProjectDlg.h"
#include "DialogCamera.h"

VideoThread::VideoThread(CamaraModule* pCamera)
{
	m_pCamera = pCamera;
}

VideoThread::~VideoThread()
{
	
}

void VideoThread::Run()
{
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	while (true)
	{
		if (GetExit() == true)
		{
			break;
		}

		pic_transit pic = m_pCamera->getpic(m_camId);

		if (pic.isselected == false)
		{
			CString str = _T("");
			str.Format(_T("���%2dδ���ӣ���������"), m_camId);
			LOG(ERROR) << MessageProcess::ProcessMessage(str);
			return;
		}

		if (pic.isok == false)
		{
			CString str = _T("");
			str.Format(_T("���%2dͼƬδ���գ�����ʧ��"), m_camId);
			LOG(ERROR) << MessageProcess::ProcessMessage(str);
			return;
		}
		//��ʾ������
		m_pMainDialog->m_DialogCamera->ShowImage(pic.pic4show);

		// �ͷ�
		if (pic.pic4show != nullptr)
		{
			delete pic.pic4show;
		}
	}

	LOG(INFO) << MessageProcess::ProcessMessage(_T("��Ƶ�ѹر�"));
}

void VideoThread::SetCamId(int id)
{
	m_camId = id;
}