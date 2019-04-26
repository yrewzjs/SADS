// DialogCamera.cpp : 实现文件
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogCamera.h"
#include "afxdialogex.h"

#include "SADSProjectDlg.h"
#include "WorkingThread.h"
#include "MessageProcess.h"
// DialogCamera 对话框

IMPLEMENT_DYNAMIC(DialogCamera, CDialogEx)

DialogCamera::DialogCamera(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogCamera::IDD, pParent)
	, m_ComboCamNum(0)
{
	//自定义
	m_isVideoOpen = false;
}

DialogCamera::~DialogCamera()
{
}

void DialogCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_COMBO_CAMERA_NUMBER, m_Combo_CameraNumber);
	DDX_Control(pDX, IDC_CAMERA_TITLE, m_CameraTitle);
	DDX_CBIndex(pDX, IDC_COMBO_CAMERA_NUMBER, m_ComboCamNum);
	DDV_MinMaxInt(pDX, m_ComboCamNum, 0, 9);
}


void DialogCamera::InitDialog()
{
	//设置标题样式
	CFont* font;
	font = m_CameraTitle.GetFont();//获取CFont对象
	LOGFONT lf;
	font->GetLogFont(&lf);//获取LOGFONT结构体
	lf.lfHeight = 30;    //修改字体大小
	lf.lfItalic = TRUE;        //倾斜
	lf.lfWeight = 50;   //修改字体的粗细

	//颜色处理

	//CFont m_Font;
	m_Font.CreateFontIndirect(&lf);//创建一个新的字体
	m_CameraTitle.SetFont(&m_Font);

	//设置combo box
	CComboBox* pComboCameraNumber = (CComboBox*)GetDlgItem(IDC_COMBO_CAMERA_NUMBER);
	pComboCameraNumber->AddString(_T("CAMERA_0"));
	pComboCameraNumber->AddString(_T("CAMERA_1"));
	pComboCameraNumber->AddString(_T("CAMERA_2"));
	pComboCameraNumber->AddString(_T("CAMERA_3"));
	pComboCameraNumber->AddString(_T("CAMERA_4"));
	pComboCameraNumber->AddString(_T("CAMERA_5"));
	pComboCameraNumber->AddString(_T("CAMERA_6"));
	pComboCameraNumber->AddString(_T("CAMERA_7"));
	pComboCameraNumber->AddString(_T("CAMERA_8"));
	pComboCameraNumber->AddString(_T("CAMERA_9"));
	pComboCameraNumber->SetCurSel(0);
}

void DialogCamera::ShowImage(CImage *image)
{
	if (image == nullptr)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("相机输出图像为NULL，请检查"));
		return;
	}

	CRect   rect;
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_PICTURE_CAMERA);//获取控件句柄
	//获取Picture Control控件的客户区
	pWnd->GetClientRect(&rect);

	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//获取picture control的DC  
	//设置指定设备环境中的位图拉伸模式
	int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	//从源矩形中复制一个位图到目标矩形，按目标设备设置的模式进行图像的拉伸或压缩
	image->StretchBlt(pDc->m_hDC, rect, SRCCOPY);
	SetStretchBltMode(pDc->m_hDC, ModeOld);

	//释放GetDC
	ReleaseDC(pDc);
}

BEGIN_MESSAGE_MAP(DialogCamera, CDialogEx)
	
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_SINGLE, &DialogCamera::OnBnClickedButtonCameraSingle)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTINUE, &DialogCamera::OnBnClickedButtonCameraContinue)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA_NUMBER, &DialogCamera::OnCbnSelchangeCamNum)
END_MESSAGE_MAP()

void DialogCamera::OnBnClickedButtonCameraSingle()
{
	// TODO:  在此添加控件通知处理程序代码
	//提交指令
	if (m_isVideoOpen == false)
	{
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_pWorkingThread->d2w_takePhoto(m_ComboCamNum);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("视频模式中，不能拍照"));
	}
}

void DialogCamera::OnBnClickedButtonCameraContinue()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_isVideoOpen == true)
	{
		this->CloseVideo();
	}
	else
	{
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_pWorkingThread->d2w_openVideo(m_ComboCamNum);
		SetDlgItemText(IDC_BUTTON_CAMERA_CONTINUE, _T("关闭视频"));
		m_isVideoOpen = true;
	}
}

void DialogCamera::OnCbnSelchangeCamNum()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将Combo box控件值更新给m_ComboCamNum
}

bool DialogCamera::GetIsVideoOpen()
{
	return m_isVideoOpen;
}

//void DialogCamera::SetIsVideoOpen(bool isopen)
//{
//	m_isVideoOpen = isopen;
//}

void DialogCamera::CloseVideo()
{
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_closeVideo();

	SetDlgItemText(IDC_BUTTON_CAMERA_CONTINUE, _T("开启视频"));
	m_isVideoOpen = false;
}