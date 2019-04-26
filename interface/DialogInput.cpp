// DialogInput.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogInput.h"
#include "afxdialogex.h"

#include "SADSProjectDlg.h"
#include "WorkingThread.h"
#include "MXComonentDef.h"
#include "MessageProcess.h"

// DialogInput �Ի���

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
	//	��ǰ��Ƭѡ��
	CComboBox* pComboInputCurSeg = (CComboBox*)GetDlgItem(IDC_COMBO_INPUT_CURSEG);
	pComboInputCurSeg->AddString(_T("0 B3"));
	pComboInputCurSeg->AddString(_T("1 B2"));
	pComboInputCurSeg->AddString(_T("2 B4"));
	pComboInputCurSeg->AddString(_T("3 B1"));
	pComboInputCurSeg->AddString(_T("4 L2"));
	pComboInputCurSeg->AddString(_T("5 L1"));
	pComboInputCurSeg->AddString(_T("6 F"));
	pComboInputCurSeg->SetCurSel(0);
	// ιƬ������Ƭѡ��
	CComboBox* pComboInputLastSeg = (CComboBox*)GetDlgItem(IDC_COMBO_INPUT_LASTSEG);
	pComboInputLastSeg->AddString(_T("0 B3"));
	pComboInputLastSeg->AddString(_T("1 B2"));
	pComboInputLastSeg->AddString(_T("2 B4"));
	pComboInputLastSeg->AddString(_T("3 B1"));
	pComboInputLastSeg->AddString(_T("4 L2"));
	pComboInputLastSeg->AddString(_T("5 L1"));
	pComboInputLastSeg->AddString(_T("6 F"));
	pComboInputLastSeg->SetCurSel(0);

	// D2060��־λ����ѡ��
	CComboBox* pComboFlagD2060 = (CComboBox*)GetDlgItem(IDC_COMBO_INPUT_2060);
	pComboFlagD2060->AddString(_T("0 �޶���"));
	pComboFlagD2060->AddString(_T("1 M63-66"));
	pComboFlagD2060->AddString(_T("2 M64-66"));
	pComboFlagD2060->AddString(_T("3 M65-66"));
	pComboFlagD2060->AddString(_T("4 M66"));
	pComboFlagD2060->AddString(_T("5 ����"));
	pComboFlagD2060->SetCurSel(0);
}
// DialogInput ��Ϣ�������

/*   K��Ƭλ����Ӧ��ť   */
void DialogInput::OnBnClickedButtonKloc()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (-18000 <= m_Edit_InputKLoc  && m_Edit_InputKLoc <= 18000)
	{
		wplc_struct *wplc = new wplc_struct;
		//ʵ��
		wplc->hasEnabled = false;
		wplc->szDevice = _T("D508");
		wplc->lpsData = m_Edit_InputKLoc;

		//�ύָ��
		CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
		m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
	}
	else
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("���벻�Ϸ������ݷ�Χ��[-18000,18000]"));
	}
}

/*   ��ǰ��Ƭ��Ӧ��ť   */
void DialogInput::OnBnClickedButtonCurseg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	wplc_struct *wplc = new wplc_struct;
	//ʵ��
	//ʹ��
	wplc->hasEnabled = true;
	wplc->enabled_szDevice = _T("D2056");
	wplc->enabled_lpsData = 1;

	//��ֵ
	wplc->szDevice = _T("D2070");
	wplc->lpsData = m_ComboInputCurSeg;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
}

/*  ĩ�˹�Ƭ��Ӧ��ť   */
void DialogInput::OnBnClickedButtonLastseg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	wplc_struct *wplc = new wplc_struct;
	//ʵ��
	//ʹ��
	wplc->hasEnabled = true;
	wplc->enabled_szDevice = _T("D2056");
	wplc->enabled_lpsData = 1;

	//��ֵ
	wplc->szDevice = _T("D2071");
	wplc->lpsData = m_ComboInputLastSeg;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
}

void DialogInput::OnBnClickedButtonInput2060()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	wplc_struct *wplc = new wplc_struct;
	//ʵ��
	wplc->hasEnabled = false;
	wplc->szDevice = _T("D2060");
	wplc->lpsData = m_ComboFlagsD2060;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
}


void DialogInput::OnBnClickedButtonInputSd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString ComdNum;
	int Yg, Sd;
	short InputValue = 0x0;
	short oneValue = 0x1;
	const int Yg_1 = oneValue << 1, Yg_2 = oneValue << 4, Yg_3 = oneValue << 7, Yg_4 = oneValue << 10;
	const int Sd_1 = oneValue << 0, Sd_2 = oneValue << 1, Sd_3 = oneValue << 2;
	if (m_Radio_Dir == 0) // ����
	{
		ComdNum = _T("D2078");
	}
	else  // ����
	{
		ComdNum = _T("D2079");
	}

	switch (m_Radio_Level)
	{
	case 0: //����
		Sd = Sd_1;
		break;
	case 1: //����
		Sd = Sd_2;
		break;
	case 2: //����
		Sd = Sd_3;
		break;
	default:
		//����
		break;
	}

	switch (m_Radio_Yg)
	{
	case 0: //��
		Yg = Yg_2;
		break;
	case 1: //��
		Yg = Yg_3;
		break;
	case 2: //����
		Yg = Yg_1;
		break;
	case 3: //��ת
		Yg = Yg_4;
		break;
	default:
		//����
		break;
	}

	wplc_struct *wplc = new wplc_struct;
	//ʵ��
	InputValue = Yg * Sd;
	wplc->hasEnabled = false;
	wplc->szDevice = ComdNum;
	wplc->lpsData = InputValue;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_inputPlc(wplc);
}

/*************************************************/

void DialogInput::OnRadioCylinderClicked()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��RADIO�ؼ�ֵ���¸�m_Radio_Yg
}


void DialogInput::OnRadioLevelClicked()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��RADIO�ؼ�ֵ���¸�m_Radio_Level
}


void DialogInput::OnRadioDireClicked()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��RADIO�ؼ�ֵ���¸�m_Radio_Dir
}



void DialogInput::OnChangeKLoc()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_Edit_InputKLoc
	//CString str;
	//str.Format(_T("xuanzhe:%d"), m_Edit_InputKLoc);
	//MessageBox(str);
}

void DialogInput::OnChangeVelocity()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_EditInputVelocity
}


void DialogInput::OnComboSelChangeFlagD2060()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_ComboFlagsD2060
}


void DialogInput::OnComboSelChangeCurSeg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_ComboInputCurSeg
}


void DialogInput::OnComboSelChangeLastSeg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_ComboInputLastSeg
}
