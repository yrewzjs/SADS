// DialogCalibration.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogCalibration.h"
#include "afxdialogex.h"

#include "SADSProjectDlg.h"
#include "WorkingThread.h"


// DialogCalibration �Ի���

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
	// PLC����1��ѡ��ť��
	ON_BN_CLICKED(IDC_RADIO_HL_1, &DialogCalibration::OnRadioGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_2K, &DialogCalibration::OnRadioGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO_HY_0, &DialogCalibration::OnRadioGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO_FY_0, &DialogCalibration::OnRadioGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO_XZ_0, &DialogCalibration::OnRadioGroup1Clicked)
	// PLC����2��ѡ��ť��
	ON_BN_CLICKED(IDC_RADIO_HD_1, &DialogCalibration::OnRadioGroup2Clicked)
	// PLC����3��ѡ��ť��
	ON_BN_CLICKED(IDC_RADIO_HL_4, &DialogCalibration::OnRadioGroup3Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_5K, &DialogCalibration::OnRadioGroup3Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_6, &DialogCalibration::OnRadioGroup3Clicked)
	ON_BN_CLICKED(IDC_RADIO_HL_7K, &DialogCalibration::OnRadioGroup3Clicked)
	// PLC����4��ѡ��ť��
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
	//����laser combo box
	CComboBox* pComboLaserCommend = (CComboBox*)GetDlgItem(IDC_COMBO_LASER_COMMENDS);
	pComboLaserCommend->AddString(_T("CALI_GENE_CATCH"));  // �궨 ��ͨ�� ץȡ
	pComboLaserCommend->AddString(_T("CALI_GENE_SPLICE")); // �궨 ��ͨ�� ƴ��
	pComboLaserCommend->AddString(_T("CALI_GENE_HEIGHT")); // �궨 ��ͨ�� ����߶�
	pComboLaserCommend->AddString(_T("CALI_ROOF_CATCH"));
	pComboLaserCommend->AddString(_T("CALI_ROOF_SPLICE"));
	pComboLaserCommend->AddString(_T("CALI_ROOF_HEIGHT"));
	pComboLaserCommend->SetCurSel(0);

	//����Camera combo box
	CComboBox* pComboCameraCommend = (CComboBox*)GetDlgItem(IDC_COMBO_CAMERA_COMMENDS);
	pComboCameraCommend->AddString(_T("CALI_GENE_CATCH_ALIGN"));  // �궨 ��ͨ�� ץȡ ��׼
	pComboCameraCommend->AddString(_T("CALI_ROOF_CATCH_ALIGN"));  // �궨 �ⶥ�� ץȡ ��׼
	pComboCameraCommend->AddString(_T("CALI_GENE_SPLICE_FIRST")); // �궨 ��ͨ�� ƴ�� ��һ��
	pComboCameraCommend->AddString(_T("CALI_GENE_SPLICE_LEFT"));  // �궨 ��ͨ�� ƴ�� ��ƴ��
	pComboCameraCommend->AddString(_T("CALI_GENE_SPLICE_RIGHT")); // �궨 ��ͨ�� ƴ�� ��ƴ��
	pComboCameraCommend->AddString(_T("CALI_ROOF_SPLICE_ALIGN")); // �궨 ��ͨ�� ƴ�� ��׼
	pComboCameraCommend->SetCurSel(0);

	//����PLC combo box
	CComboBox* pComboCaliPlc = (CComboBox*)GetDlgItem(IDC_COMBO_CALI_PLC);
	pComboCaliPlc->AddString(_T("��ת"));
	pComboCaliPlc->AddString(_T("����"));
	pComboCaliPlc->AddString(_T("��ҡ"));
	pComboCaliPlc->AddString(_T("�����"));
	pComboCaliPlc->AddString(_T("������"));
	pComboCaliPlc->AddString(_T("��ת��ֵ"));
	pComboCaliPlc->SetCurSel(0);
}

// DialogCalibration ��Ϣ�������

/*  ����궨����ȷ�ϰ�ť  */
void DialogCalibration::OnBnClickedButtonCaliLaser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliLaser(m_ComboLaserCommend);
}

/*  ͼ��궨����ȷ�ϰ�ť  */
void DialogCalibration::OnBnClickedButtonCaliCamera()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliCamera(m_ComboCamCommend);
}

/*  PLC ��һ����ȷ�ϰ�ť  */
void DialogCalibration::OnBnClickedButtonPlc1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	short oneValue = 0x1;
	short InputValue = 0x1;
	switch (m_Radio_Group1)
	{
	case 0: //����1
		InputValue = oneValue << 1;
		break;
	case 1: //����2K
		InputValue = oneValue << 2;
		break;
	case 2: //��ҡ0
		InputValue = oneValue << 5;
		break;
	case 3: //����0
		InputValue = oneValue << 4;
		break;
	case 4: //��ת0
		InputValue = oneValue << 3;
		break;
	default:
		//����
		break;
	}
	// ����
	wplc_struct *wplc = new wplc_struct;
	wplc->hasEnabled = false;
	wplc->szDevice = _T("D2062");
	wplc->lSize = 1;
	wplc->lpsData = InputValue;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc);
}
/*  PLC �ڶ�����ȷ�ϰ�ť  */
void DialogCalibration::OnBnClickedButtonPlc2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	short InputValue = 0x1;
	short oneValue = 0x1;
	switch (m_Radio_Group2)
	{
	case 0: //����1
		InputValue = oneValue << 1;
		break;
	default:
		break;
	}

	//ָ��
	wplc_struct *wplc1 = new wplc_struct;
	wplc1->hasEnabled = false;
	wplc1->szDevice = _T("D2063");
	wplc1->lpsData = InputValue;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc1);
}

/*  PLC ��������ȷ�ϰ�ť  */
void DialogCalibration::OnBnClickedButtonPlc3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	short oneValue = 0x1;
	short InputValue = 0x1;
	switch (m_Radio_Group1)
	{
	case 0: //����4
		InputValue = oneValue << 1;
		break;
	case 1: //����5K
		InputValue = oneValue << 2;
		break;
	case 2: //����6
		InputValue = oneValue << 3;
		break;
	case 3: //����7K
		InputValue = oneValue << 4;
		break;
	default:
		break;
	}

	wplc_struct *wplc1 = new wplc_struct;
	//ʵ��
	//���ύ��ѡ�򲿷�ָ��
	wplc1->hasEnabled = false;
	wplc1->szDevice = _T("D2064");
	wplc1->lpsData = InputValue;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc1);
}

/*  PLC ��������ȷ�ϰ�ť  */
void DialogCalibration::OnBnClickedButtonPlc4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	short oneValue = 0x1;
	short InputValue = 0x1;
	switch (m_Radio_Group1)
	{
	case 0: //����8
		InputValue = oneValue << 3;
		break;
	case 1: //����9K
		InputValue = oneValue << 4;
		break;
	case 2: //����2
		InputValue = oneValue << 1;
		break;
	case 3: //����3K
		InputValue = oneValue << 2;
		break;
	default:
		break;
	}

	wplc_struct *wplc1 = new wplc_struct;
	//ָ��
	wplc1->hasEnabled = false;
	wplc1->szDevice = _T("D2065");
	wplc1->lpsData = InputValue;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc1);
}

void DialogCalibration::OnBnClickedButtonPlc5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString en_szDevice; // ʹ��λ
	CString szDevice; // ��־λ
	short oneValue = 0x1;
	short en_Value = 0x1; // ʹ����ֵ
	switch (m_ComboCaliPlc)
	{
	case 0: //��ת
		en_szDevice = _T("D2066");
		szDevice = _T("D2072");
		en_Value = oneValue << 1;
		break;
	case 1: //����
		en_szDevice = _T("D2066");
		szDevice = _T("D2073");
		en_Value = oneValue << 2;
		break;
	case 2: //��ҡ
		en_szDevice = _T("D2066");
		szDevice = _T("D2074");
		en_Value = oneValue << 3;
		break;
	case 3: //�����
		en_szDevice = _T("D2067");
		szDevice = _T("D2075");
		en_Value = oneValue << 1;
		break;
	case 4: //������
		en_szDevice = _T("D2068");
		szDevice = _T("D2076");
		en_Value = oneValue << 1;
		break;
	case 5: //��ת��ֵ
		en_szDevice = _T("D2069");
		szDevice = _T("D2077");
		en_Value = oneValue << 1;
		break;
	default:
		break;
	}

	wplc_struct *wplc1 = new wplc_struct;
	//ָ��
	wplc1->hasEnabled = true;
	wplc1->enabled_szDevice = en_szDevice;
	wplc1->enabled_lSize = 1;
	wplc1->enabled_lpsData = en_Value;

	wplc1->szDevice = szDevice;
	wplc1->lSize = 1;
	wplc1->lpsData = m_EditCaliPlc;

	//�ύָ��
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_pWorkingThread->d2w_caliPlc(wplc1);
}
/*******************************************************/

void DialogCalibration::OnRadioGroup1Clicked()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_Radio_Group1
	//CString str;
	//str.Format(_T("xuanzhe:%d"), m_Radio_Group1);
	//MessageBox(str);
}


void DialogCalibration::OnRadioGroup2Clicked()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_Radio_Group2
}


void DialogCalibration::OnRadioGroup3Clicked()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_Radio_Group3
}


void DialogCalibration::OnRadioGroup4Clicked()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_Radio_Group4
}


void DialogCalibration::OnCbnSelChangeLaserCommend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_ComboLaserCommend
}


void DialogCalibration::OnCbnSelChangeCamCommend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_ComboCamCommend
}


void DialogCalibration::OnCbnSelChangeCaliPlc()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_ComboCaliPlc
}

void DialogCalibration::OnEnChangeEditCaliPlc()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ���ؼ�ֵ���¸�m_EditCaliPlc
}
