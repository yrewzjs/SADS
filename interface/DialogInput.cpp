// DialogInput.cpp : 实现文件
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogInput.h"
#include "afxdialogex.h"

#include "SADSProjectDlg.h"
#include "WorkingThread.h"
#include "MXComonentDef.h"
#include "MessageProcess.h"

// DialogInput 对话框

IMPLEMENT_DYNAMIC(DialogInput, CDialogEx)

DialogInput::DialogInput(CWnd* pParent /*=NULL*/)
: CDialogEx(DialogInput::IDD, pParent)
, m_Radio_Yg(0)
, m_Radio_Level(0)
, m_Radio_Dir(0)
, m_Edit_InputKLoc(0)
, m_EditInputVelocity(0)
, m_ComboFlagsD2060(0)
, m_ComboInputCurSeg(0)
, m_ComboInputLastSeg(0)
{

}

DialogInput::~DialogInput()
{
}

void DialogInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_INPUT_RED, m_Radio_Yg);
	DDV_MinMaxInt(pDX, m_Radio_Yg, 0, 3);
	DDX_Radio(pDX, IDC_RADIO_INPUT_HIGH, m_Radio_Level);
	DDV_MinMaxInt(pDX, m_Radio_Level, 0, 2);
	DDX_Radio(pDX, IDC_RADIO_INPUT_TYPE_SHEN, m_Radio_Dir);
	DDV_MinMaxInt(pDX, m_Radio_Dir, 0, 1);
	DDX_Text(pDX, IDC_EDIT_INPUT_KLOC, m_Edit_InputKLoc);
	DDV_MinMaxInt(pDX, m_Edit_InputKLoc, -18000, 18000);
	DDX_Text(pDX, IDC_EDIT_INPUT_SHOWSD, m_EditInputVelocity);
	DDV_MinMaxInt(pDX, m_EditInputVelocity, 0, 255);
	DDX_CBIndex(pDX, IDC_COMBO_INPUT_2060, m_ComboFlagsD2060);
	DDV_MinMaxInt(pDX, m_ComboFlagsD2060, 0, 5);
	DDX_CBIndex(pDX, IDC_COMBO_INPUT_CURSEG, m_ComboInputCurSeg);
	DDV_MinMaxInt(pDX, m_ComboInputCurSeg, 0, 6);
	DDX_CBIndex(pDX, IDC_COMBO_INPUT_LASTSEG, m_ComboInputLastSeg);
	DDV_MinMaxInt(pDX, m_ComboInputLastSeg, 0, 6);
}


BEGIN_MESSAGE_MAP(DialogInput, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_KLOC, &DialogInput::OnBnClickedButtonKloc)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_CURSEG, &DialogInput::OnBnClickedButtonCurseg)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_LASTSEG, &DialogInput::OnBnClickedButtonLastseg)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_2060, &DialogInput::OnBnClickedButtonInput2060)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_SD, &DialogInput::OnBnClickedButtonInputSd)

	ON_BN_CLICKED(IDC_RADIO_INPUT_RED, &DialogInput::OnRadioCylinderClicked)
	ON_BN_CLICKED(IDC_RADIO_INPUT_BLUE, &DialogInput::OnRadioCylinderClicked)
	ON_BN_CLICKED(IDC_RADIO_INPUT_HD, &DialogInput::OnRadioCylinderClicked)
	ON_BN_CLICKED(IDC_RADIO_INPUT_XZ, &DialogInput::OnRadioCylinderClicked)

	ON_BN_CLICKED(IDC_RADIO_INPUT_HIGH, &DialogInput::OnRadioLevelClicked)
	ON_BN_CLICKED(IDC_RADIO_INPUT_MID, &DialogInput::OnRadioLevelClicked)
	ON_BN_CLICKED(IDC_RADIO_INPUT_LOW, &DialogInput::OnRadioLevelClicked)

	ON_BN_CLICKED(IDC_RADIO_INPUT_TYPE_SHEN, &DialogInput::OnRadioDireClicked)
	ON_BN_CLICKED(IDC_RADIO_INPUT_TYPE_SUO, &DialogInput::OnRadioDireClicked)

	ON_EN_CHANGE(IDC_EDIT_INPUT_KLOC, &DialogInput::OnChangeKLoc)
	ON_EN_CHANGE(IDC_EDIT_INPUT_SHOWSD, &DialogInput::OnChangeVelocity)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_2060, &DialogInput::OnComboSelChangeFlagD2060)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_CURSEG, &DialogInput::OnComboSelChangeCurSeg)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_LASTSEG, &DialogInput::OnComboSelChangeLastSeg)
END_MESSAGE_MAP()


void DialogInput::InitDialog()
{
	//	当前管片选项
	CComboBox* pComboInputCurSeg = (CComboBox*)GetDlgItem(IDC_COMBO_INPUT_CURSEG);
	pComboInputCurSeg->AddString(_T("0 B3"));
	pComboInputCurSeg->AddString(_T("1 B2"));
	pComboInputCurSeg->AddString(_T("2 B4"));
	pComboInputCurSeg->AddString(_T("3 B1"));
	pComboInputCurSeg->AddString(_T("4 L2"));
	pComboInputCurSeg->AddString(_T("5 L1"));
	pComboInputCurSeg->AddString(_T("6 F"));
	pComboInputCurSeg->SetCurSel(0);
	// 喂片机最后管片选项
	CComboBox* pComboInputLastSeg = (CComboBox*)GetDlgItem(IDC_COMBO_INPUT_LASTSEG);
	pComboInputLastSeg->AddString(_T("0 B3"));
	pComboInputLastSeg->AddString(_T("1 B2"));
	pComboInputLastSeg->AddString(_T("2 B4"));
	pComboInputLastSeg->AddString(_T("3 B1"));
	pComboInputLastSeg->AddString(_T("4 L2"));
	pComboInputLastSeg->AddString(_T("5 L1"));
	pComboInputLastSeg->AddString(_T("6 F"));
	pComboInputLastSeg->SetCurSel(0);

	// D2060标志位设置选项
	CComboBox* pComboFlagD2060 = (CComboBox*)GetDlgItem(IDC_COMBO_INPUT_2060);
	pComboFlagD2060->AddString(_T("0 无动作"));
	pComboFlagD2060->AddString(_T("1 M63-66"));
	pComboFlagD2060->AddString(_T("2 M64-66"));
	pComboFlagD2060->AddString(_T("3 M65-66"));
	pComboFlagD2060->AddString(_T("4 M66"));
	pComboFlagD2060->AddString(_T("5 重启"));
	pComboFlagD2060->SetCurSel(0);
}
// DialogInput 消息处理程序

/*   K管片位置响应按钮   */
void DialogInput::OnBnClickedButtonKloc()
{
	// TODO:  在此添加控件通知处理程序代码
	if (-18000 <= m_Edit_InputKLoc  && m_Edit_InputKLoc <= 18000)
	{
		wplc_struct *wplc = new wplc_struct;
		//实现
		wplc->hasEnabled = false;
		wplc->szDevice = _T("D508");
		wplc->lpsData = m_Edit_InputKLoc;

		//提交指令
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("输入不合法，数据范围：[-18000,18000]"));
	}
}

/*   当前管片响应按钮   */
void DialogInput::OnBnClickedButtonCurseg()
{
	// TODO:  在此添加控件通知处理程序代码
	wplc_struct *wplc = new wplc_struct;
	//实现
	//使能
	wplc->hasEnabled = true;
	wplc->enabled_szDevice = _T("D2056");
	wplc->enabled_lpsData = 1;

	//数值
	wplc->szDevice = _T("D2070");
	wplc->lpsData = m_ComboInputCurSeg;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
}

/*  末端管片响应按钮   */
void DialogInput::OnBnClickedButtonLastseg()
{
	// TODO:  在此添加控件通知处理程序代码
	wplc_struct *wplc = new wplc_struct;
	//实现
	//使能
	wplc->hasEnabled = true;
	wplc->enabled_szDevice = _T("D2056");
	wplc->enabled_lpsData = 1;

	//数值
	wplc->szDevice = _T("D2071");
	wplc->lpsData = m_ComboInputLastSeg;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
}

void DialogInput::OnBnClickedButtonInput2060()
{
	// TODO:  在此添加控件通知处理程序代码
	wplc_struct *wplc = new wplc_struct;
	//实现
	wplc->hasEnabled = false;
	wplc->szDevice = _T("D2060");
	wplc->lpsData = m_ComboFlagsD2060;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
}


void DialogInput::OnBnClickedButtonInputSd()
{
	// TODO:  在此添加控件通知处理程序代码
	CString ComdNum;
	int Yg, Sd;
	short InputValue = 0x0;
	short oneValue = 0x1;
	const int Yg_1 = oneValue << 1, Yg_2 = oneValue << 4, Yg_3 = oneValue << 7, Yg_4 = oneValue << 10;
	const int Sd_1 = oneValue << 0, Sd_2 = oneValue << 1, Sd_3 = oneValue << 2;
	if (m_Radio_Dir == 0) // 伸右
	{
		ComdNum = _T("D2078");
	}
	else  // 缩左
	{
		ComdNum = _T("D2079");
	}

	switch (m_Radio_Level)
	{
	case 0: //高速
		Sd = Sd_1;
		break;
	case 1: //中速
		Sd = Sd_2;
		break;
	case 2: //低速
		Sd = Sd_3;
		break;
	default:
		//报错
		break;
	}

	switch (m_Radio_Yg)
	{
	case 0: //红
		Yg = Yg_2;
		break;
	case 1: //蓝
		Yg = Yg_3;
		break;
	case 2: //滑动
		Yg = Yg_1;
		break;
	case 3: //旋转
		Yg = Yg_4;
		break;
	default:
		//报错
		break;
	}

	wplc_struct *wplc = new wplc_struct;
	//实现
	InputValue = Yg * Sd;
	wplc->hasEnabled = false;
	wplc->szDevice = ComdNum;
	wplc->lpsData = InputValue;

	//提交指令
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
}

/*************************************************/

void DialogInput::OnRadioCylinderClicked()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将RADIO控件值更新给m_Radio_Yg
}


void DialogInput::OnRadioLevelClicked()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将RADIO控件值更新给m_Radio_Level
}


void DialogInput::OnRadioDireClicked()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将RADIO控件值更新给m_Radio_Dir
}



void DialogInput::OnChangeKLoc()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_Edit_InputKLoc
	//CString str;
	//str.Format(_T("xuanzhe:%d"), m_Edit_InputKLoc);
	//MessageBox(str);
}

void DialogInput::OnChangeVelocity()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_EditInputVelocity
}


void DialogInput::OnComboSelChangeFlagD2060()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_ComboFlagsD2060
}


void DialogInput::OnComboSelChangeCurSeg()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_ComboInputCurSeg
}


void DialogInput::OnComboSelChangeLastSeg()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 将控件值更新给m_ComboInputLastSeg
}
