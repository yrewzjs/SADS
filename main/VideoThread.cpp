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
			str.Format(_T("相机%2d未连接，不能拍照"), m_camId);
			LOG(ERROR) << MessageProcess::ProcessMessage(str);
			return;
		}

		if (pic.isok == false)
		{
			CString str = _T("");
			str.Format(_T("相机%2d图片未接收，拍照失败"), m_camId);
			LOG(ERROR) << MessageProcess::ProcessMessage(str);
			return;
		}
		//显示到界面
		m_pMainDialog->m_DialogCamera->ShowImage(pic.pic4show);

		// 释放
		if (pic.pic4show != nullptr)
		{
			delete pic.pic4show;
		}
	}

	LOG(INFO) << MessageProcess::ProcessMessage(_T("视频已关闭"));
}

void VideoThread::SetCamId(int id)
{
	m_camId = id;
}