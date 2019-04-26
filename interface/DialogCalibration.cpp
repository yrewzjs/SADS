// DialogCalibration.cpp : 实现文件
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogCalibration.h"
#include "afxdialogex.h"

#include "SADSProjectDlg.h"
#include "WorkingThread.h"


// DialogCalibration 对话框

IMPLEMENT_DYNAMIC(DialogCalibration, CDialogEx)

DialogCalibration::DialogCalibration(CWnd* pParent /*=NULL*/)
: CDialogEx(DialogCalibration::IDD, pParent)
, m_Radio_Group1(0)
, m_Radio_Group2(0)
, m_Radio_Group3(0)
, m_Radio_Group4(0)
, m_ComboLaserCommend(0)
, m_ComboCamCommend(0)
, m_ComboCaliPlc(0)
, m_EditCaliPlc(0)
{

}

DialogCalibration::~DialogCalibration()
{

}

void DialogCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_HL_1, m_Radio_Group1);
	DDV_MinMaxInt(pDX, m_Radio_Group1, 0, 4);
	DDX_Radio(pDX, IDC_RADIO_HD_1, m_Radio_Group2);
	DDV_MinMaxInt(pDX, m_Radio_Group2, 0, 0);
	DDX_Radio(pDX, IDC_RADIO_HL_4, m_Radio_Group3);
	DDV_MinMaxInt(pDX, m_Radio_Group3, 0, 3);
	DDX_Radio(pDX, IDC_RADIO_HL_8, m_Radio_Group4);
	DDV_MinMaxInt(pDX, m_Radio_Group4, 0, 3);
	DDX_CBIndex(pDX, IDC_COMBO_LASER_COMMENDS, m_ComboLaserCommend);
	DDV_MinMaxInt(pDX, m_ComboLaserCommend, 0, 5);
	DDX_CBIndex(pDX, IDC_COMBO_CAMERA_COMMENDS, m_ComboCamCommend);
	DDV_MinMaxInt(pDX, m_ComboCamCommend, 0, 5);
	DDX_CBIndex(pDX, IDC_COMBO_CALI_PLC, m_ComboCaliPlc);
	DDV_MinMaxInt(pDX, m_ComboCaliPlc, 0, 5);
	DDX_Text(pDX, IDC_EDIT_CALI_PLC, m_EditCaliPlc);
	DDV_MinMaxInt(pDX, m_EditCaliPlc, 0, 100);
}

BEGIN_MESSAGE_MAP(DialogCalibration, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CALI_LASER, &DialogCalibration::OnBnClickedButtonCaliLaser)
	ON_BN_CLICKED(IDC_BUTTON_CALI_CAMERA, &DialogCalibration::OnBnClickedButtonCaliCamera)
	ON_BN_CLICKED(IDC_BUTTON_PLC_1, &DialogCalibration::OnBnClickedButtonPlc1)
	ON_BN_CLICKED(IDC_BUTTON_PLC_2, &DialogCalibration::OnBnClickedButtonPlc2)
	ON_BN_CLICKED(IDC_BUTTON_PLC_3, &DialogCalibration::OnBnClickedButtonPlc3)
	ON_BN_CLICKED(IDC_BUTTON_PLC_4, &DialogCalibration::OnBnClickedButtonPlc4)
	ON_BN_CLICKED(IDC_BUTTON_PLC_5, &DialogCalibration::OnBnClickedButtonPlc5)
	// PLC区域1单选按钮组
	ON_BN_CLICKED(IDC_RADIO_HL_1, &DialogCalibration::OnRadioGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_2K, &DialogCalibration::OnRadioGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO_HY_0, &DialogCalibration::OnRadioGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO_FY_0, &DialogCalibration::OnRadioGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO_XZ_0, &DialogCalibration::OnRadioGroup1Clicked)
	// PLC区域2单选按钮组
	ON_BN_CLICKED(IDC_RADIO_HD_1, &DialogCalibration::OnRadioGroup2Clicked)
	// PLC区域3单选按钮组
	ON_BN_CLICKED(IDC_RADIO_HL_4, &DialogCalibration::OnRadioGroup3Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_5K, &DialogCalibration::OnRadioGroup3Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_6, &DialogCalibration::OnRadioGroup3Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_7K, &DialogCalibration::OnRadioGroup3Clicked)
	// PLC区域4单选按钮组
	ON_BN_CLICKED(IDC_RADIO_HL_8, &DialogCalibration::OnRadioGroup4Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_9K, &DialogCalibration::OnRadioGroup4Clicked)
	ON_BN_CLICKED(IDC_RADIO_HD_2, &DialogCalibration::OnRadioGroup4Clicked)
	ON_BN_CLICKED(IDC_RADIO_HD_3K, &DialogCalibration::OnRadioGroup4Clicked)

	ON_CBN_SELCHANGE(IDC_COMBO_LASER_COMMENDS, &DialogCalibration::OnCbnSelChangeLaserCommend)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA_COMMENDS, &DialogCalibration::OnCbnSelChangeCamCommend)
	ON_CBN_SELCHANGE(IDC_COMBO_CALI_PLC, &DialogCalibration::OnCbnSelChangeCaliPlc)

	ON_EN_CHANGE(IDC_EDIT_CALI_PLC, &DialogCalibration::OnEnChangeEditCaliPlc)
END_MESSAGE_MAP()


void DialogCalibration::InitDialog()
{
	//设置laser combo box
	CComboBox* pComboLaserCommend = (CComboBox*)GetDlgItem(IDC_COMBO_LASER_COMMENDS);
	pComboLaserCommend->AddString(_T("CALI_GENE_CATCH"));  // 标定 普通块 抓取
	pComboLaserCommend->AddString(_T("CALI_GENE_SPLICE")); // 标定 普通块 拼接
	pComboLaserCommend->AddString(_T("CALI_GENE_HEIGHT")); // 标定 普通块 相机高度
	pComboLaserCommend->AddString(_T("CALI_ROOF_CATCH"));
	pComboLaserCommend->AddString(_T("CALI_ROOF_SPLICE"));
	pComboLaserCommend->AddString(_T("CALI_ROOF_HEIGHT"));
	pComboLaserCommend->SetCurSel(0);

	//设置Camera combo box
	CComboBox* pComboCameraCommend = (CComboBox*)GetDlgItem(IDC_COMBO_CAMERA_COMMENDS);
	pComboCameraCommend->AddString(_T("CALI_GENE_CATCH_ALIGN"));  // 标定 普通块 抓取 对准
	pComboCameraCommend->AddString(_T("CALI_ROOF_CATCH_ALIGN"));  // 标定 封顶块 抓取 对准
	pComboCameraCommend->AddString(_T("CALI_GENE_SPLICE_FIRST")); // 标定 普通块 拼接 第一块
	pComboCameraCommend->AddString(_T("CALI_GENE_SPLICE_LEFT"));  // 标定 普通块 拼接 左拼接
	pComboCameraCommend->AddString(_T("CALI_GENE_SPLICE_RIGHT")); // 标定 普通块 拼接 右拼接
	pComboCameraCommend->AddString(_T("CALI_ROOF_SPLICE_ALIGN")); // 标定 普通块 拼接 对准
	pComboCameraCommend->SetCurSel(0);

	//设置PLC combo box
	CComboBox* pComboCaliPlc = (CComboBox*)GetDlgItem(IDC_COMBO_CALI_PLC);
	pComboCaliPlc->AddString(_T("旋转"));
	pComboCaliPlc->AddString(_T("俯仰"));
	pComboCaliPlc->AddString(_T("横摇"));
	pComboCaliPlc->AddString(_T("红对蓝"));
	pComboCaliPlc->AddString(_T("红蓝续"));
	pComboCaliPlc->AddString(_T("旋转阈值"));
	pComboCaliPlc->SetCurSel(0);
}

// DialogCalibration 消息处理程序

/*  激光标定区域确认按钮  */
void DialogCalibration::OnBnClickedButtonCaliLaser()
{
	// TODO:  在此添加控件通知处理程序代码
	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliLaser(m_ComboLaserCommend);
}

/*  图像标定区域确认按钮  */
void DialogCalibration::OnBnClickedButtonCaliCamera()
{
	// TODO:  在此添加控件通知处理程序代码
	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliCamera(m_ComboCamCommend);
}

/*  PLC 第一区域确认按钮  */
void DialogCalibration::OnBnClickedButtonPlc1()
{
	// TODO:  在此添加控件通知处理程序代码
	short oneValue = 0x1;
	short InputValue = 0x1;
	switch (m_Radio_Group1)
	{
	case 0: //红蓝1
		InputValue = oneValue << 1;
		break;
	case 1: //红蓝2K
		InputValue = oneValue << 2;
		break;
	case 2: //横摇0
		InputValue = oneValue << 5;
		break;
	case 3: //俯仰0
		InputValue = oneValue << 4;
		break;
	case 4: //旋转0
		InputValue = oneValue << 3;
		break;
	default:
		//报错
		break;
	}
	// 命令
	wplc_struct *wplc = new wplc_struct;
	wplc->hasEnabled = false;
	wplc->szDevice = _T("D2062");
	wplc->lSize = 1;
	wplc->lpsData = InputValue;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc);
}
/*  PLC 第二区域确认按钮  */
void DialogCalibration::OnBnClickedButtonPlc2()
{
	// TODO:  在此添加控件通知处理程序代码
	short InputValue = 0x1;
	short oneValue = 0x1;
	switch (m_Radio_Group2)
	{
	case 0: //滑动1
		InputValue = oneValue << 1;
		break;
	default:
		break;
	}

	//指令
	wplc_struct *wplc1 = new wplc_struct;
	wplc1->hasEnabled = false;
	wplc1->szDevice = _T("D2063");
	wplc1->lpsData = InputValue;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc1);
}

/*  PLC 第三区域确认按钮  */
void DialogCalibration::OnBnClickedButtonPlc3()
{
	// TODO:  在此添加控件通知处理程序代码
	short oneValue = 0x1;
	short InputValue = 0x1;
	switch (m_Radio_Group1)
	{
	case 0: //红蓝4
		InputValue = oneValue << 1;
		break;
	case 1: //红蓝5K
		InputValue = oneValue << 2;
		break;
	case 2: //红蓝6
		InputValue = oneValue << 3;
		break;
	case 3: //红蓝7K
		InputValue = oneValue << 4;
		break;
	default:
		break;
	}

	wplc_struct *wplc1 = new wplc_struct;
	//实现
	//先提交单选框部分指令
	wplc1->hasEnabled = false;
	wplc1->szDevice = _T("D2064");
	wplc1->lpsData = InputValue;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc1);
}

/*  PLC 第四区域确认按钮  */
void DialogCalibration::OnBnClickedButtonPlc4()
{
	// TODO:  在此添加控件通知处理程序代码
	short oneValue = 0x1;
	short InputValue = 0x1;
	switch (m_Radio_Group1)
	{
	case 0: //红蓝8
		InputValue = oneValue << 3;
		break;
	case 1: //红蓝9K
		InputValue = oneValue << 4;
		break;
	case 2: //滑动2
		InputValue = oneValue << 1;
		break;
	case 3: //滑动3K
		InputValue = oneValue << 2;
		break;
	default:
		break;
	}

	wplc_struct *wplc1 = new wplc_struct;
	//指令
	wplc1->hasEnabled = false;
	wplc1->szDevice = _T("D2065");
	wplc1->lpsData = InputValue;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc1);
}

void DialogCalibration::OnBnClickedButtonPlc5()
{
	// TODO:  在此添加控件通知处理程序代码
	CString en_szDevice; // 使能位
	CString szDevice; // 标志位
	short oneValue = 0x1;
	short en_Value = 0x1; // 使能数值
	switch (m_ComboCaliPlc)
	{
	case 0: //旋转
		en_szDevice = _T("D2066");
		szDevice = _T("D2072");
		en_Value = oneValue << 1;
		break;
	case 1: //俯仰
		en_szDevice = _T("D2066");
		szDevice = _T("D2073");
		en_Value = oneValue << 2;
		break;
	case 2: //横摇
		en_szDevice = _T("D2066");
		szDevice = _T("D2074");
		en_Value = oneValue << 3;
		break;
	case 3: //红对蓝
		en_szDevice = _T("D2067");
		szDevice = _T("D2075");
		en_Value = oneValue << 1;
		break;
	case 4: //红蓝续
		en_szDevice = _T("D2068");
		szDevice = _T("D2076");
		en_Value = oneValue << 1;
		break;
	case 5: //旋转阈值
		en_szDevice = _T("D2069");
		szDevice = _T("D2077");
		en_Value = oneValue << 1;
		break;
	default:
		break;
	}

	wplc_struct *wplc1 = new wplc_struct;
	//指令
	wplc1->hasEnabled = true;
	wplc1->enabled_szDevice = en_szDevice;
	wplc1->enabled_lSize = 1;
	wplc1->enabled_lpsData = en_Value;

	wplc1->szDevice = szDevice;
	wplc1->lSize = 1;
	wplc1->lpsData = m_EditCaliPlc;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc1);
}
/*******************************************************/

void DialogCalibration::OnRadioGroup1Clicked()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_Radio_Group1
	//CString str;
	//str.Format(_T("xuanzhe:%d"), m_Radio_Group1);
	//MessageBox(str);
}


void DialogCalibration::OnRadioGroup2Clicked()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_Radio_Group2
}


void DialogCalibration::OnRadioGroup3Clicked()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_Radio_Group3
}


void DialogCalibration::OnRadioGroup4Clicked()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_Radio_Group4
}


void DialogCalibration::OnCbnSelChangeLaserCommend()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_ComboLaserCommend
}


void DialogCalibration::OnCbnSelChangeCamCommend()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_ComboCamCommend
}


void DialogCalibration::OnCbnSelChangeCaliPlc()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_ComboCaliPlc
}

void DialogCalibration::OnEnChangeEditCaliPlc()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_EditCaliPlc
}
