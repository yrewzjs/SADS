
// SADSProjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SADSProject.h"
#include "SADSProjectDlg.h"
#include "afxdialogex.h"
#include "tinystr.h"   
#include "tinyxml.h"

#include "DialogCalibration.h"
#include "DialogInput.h"
#include "DialogCamera.h"
#include "DialogDisplayPartA.h"
#include "DialogDisplayPartB.h"
#include "DialogDisplayPartC.h"

#include "WorkingThread.h"
#include "MessageProcess.h"
#include "MxWrapper.h"
#include "AdjustThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSADSProjectDlg �Ի���



CSADSProjectDlg::CSADSProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSADSProjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//�Զ���
	//��ʼ����־���
	MessageProcess::InitLogConfiguration();
	//����working�̣߳����߳�ֻ�������Ի����ʼ��ʱ�������ڳ����˳�ʱ�˳�
	m_pWorkingThread = new WorkingThread();
	m_pWorkingThread->Start();
}

CSADSProjectDlg::~CSADSProjectDlg()
{
	if (m_DialogCalibration != nullptr)
	{
		delete m_DialogCalibration;
		m_DialogCalibration = nullptr;
	}

	if (m_DialogInput != nullptr)
	{
		delete m_DialogInput;
		m_DialogInput = nullptr;
	}

	if (m_DialogCamera != nullptr)
	{
		delete m_DialogCamera;
		m_DialogCamera = nullptr;
	}

	if (m_DialogDisplayA != nullptr)
	{
		delete m_DialogDisplayA;
		m_DialogDisplayA = nullptr;
	}

	if (m_DialogDisplayB != nullptr)
	{
		delete m_DialogDisplayB;
		m_DialogDisplayB = nullptr;
	}

	if (m_DialogDisplayC != nullptr)
	{
		delete m_DialogDisplayC;
		m_DialogDisplayC = nullptr;
	}
}

void CSADSProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_ACTUTLTYPE_MX, m_ActUtl_Mx);
}

BEGIN_MESSAGE_MAP(CSADSProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//�Զ�����¼���Ӧӳ��

	// ��һ������Ϊ�˵���ID,�ڶ�������Ϊ�¼���Ӧ��������;
	ON_COMMAND(ID_MENU_INT_MSG, &CSADSProjectDlg::OnClickedMenuInterfaceMessage)
	ON_COMMAND(ID_MENU_INT_CALI, &CSADSProjectDlg::OnClickedMenuInterfaceCalibration)
	ON_COMMAND(ID_MENU_INT_CAM, &CSADSProjectDlg::OnClickedMenuInterfaceCamera)
	ON_COMMAND(ID_MENU_INT_INPUT, &CSADSProjectDlg::OnClickedMenuInterfaceInput)

	ON_COMMAND(ID_MENU_SYS_OPEN, &CSADSProjectDlg::OnMenuSysOpen)
	ON_COMMAND(ID_MENU_SYS_CLOSE, &CSADSProjectDlg::OnMenuSysClose)
	ON_COMMAND(ID_MENU_SYS_EXIT, &CSADSProjectDlg::OnMenuSysExit)
	ON_COMMAND(ID_MENU_ABOUT, &CSADSProjectDlg::OnMenuAbout)
	ON_COMMAND(ID_MENU_SYS_STOPAUTO, &CSADSProjectDlg::OnMenuSysStopauto)
	ON_COMMAND(ID_MENU_OPENAUTOCATCH, &CSADSProjectDlg::OnMenuOpenAutoCatch)
END_MESSAGE_MAP()


// CSADSProjectDlg ��Ϣ�������

BOOL CSADSProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//�رհ�ťʧЧ
	CMenu *pMenu = this->GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);     //�رհ�ť���
	pMenu->ModifyMenu(SC_CLOSE,MF_BYCOMMAND | MF_GRAYED);  // �رհ�ť����Ч
	// ��ʾ��dialog
	ShowWindow(SW_NORMAL);

	//��ʾdisplayA����
	CRect rect_displayA;
	GetDlgItem(IDC_PICTURE_PART_A)->GetWindowRect(&rect_displayA);
	ScreenToClient(&rect_displayA);
	m_DialogDisplayA = new DialogDisplayPartA();
	m_DialogDisplayA->Create(IDD_DIALOG_DISPLAYPART_A, this);
	m_DialogDisplayA->MoveWindow(rect_displayA.left, rect_displayA.top, rect_displayA.Width(), rect_displayA.Height());
	m_DialogDisplayA->ShowWindow(SW_SHOW);
	m_DialogDisplayA->InitDialog();

	//��ʾdisplayB����
	CRect rect_displayB;
	GetDlgItem(IDC_PICTURE_PART_B)->GetWindowRect(&rect_displayB);
	ScreenToClient(&rect_displayB);
	m_DialogDisplayB = new DialogDisplayPartB();
	m_DialogDisplayB->Create(IDD_DIALOG_DISPLAYPART_B, this);
	m_DialogDisplayB->MoveWindow(rect_displayB.left, rect_displayB.top, rect_displayB.Width(), rect_displayB.Height());
	m_DialogDisplayB->ShowWindow(SW_SHOW);
	m_DialogDisplayB->InitDialog();

	//��ʾdisplayC����
	CRect rect_displayC;
	GetDlgItem(IDC_PICTURE_PART_C)->GetWindowRect(&rect_displayC);
	ScreenToClient(&rect_displayC);
	m_DialogDisplayC = new DialogDisplayPartC();
	m_DialogDisplayC->Create(IDD_DIALOG_DISPLAYPART_C, this);
	m_DialogDisplayC->MoveWindow(rect_displayC.left, rect_displayC.top, rect_displayC.Width(), rect_displayC.Height());
	m_DialogDisplayC->ShowWindow(SW_SHOW);
	m_DialogDisplayC->InitDialog();
	
	//��ʾcamera����
	CRect rect_camera;
	GetDlgItem(IDC_PICTRUE_MAIN_CAMERA)->GetWindowRect(&rect_camera);
	ScreenToClient(&rect_camera);
	m_DialogCamera = new DialogCamera();
	m_DialogCamera->Create(IDD_DIALOG_CAMERA, this);
	m_DialogCamera->MoveWindow(rect_camera.left, rect_camera.top, rect_camera.Width(), rect_camera.Height());
	m_DialogCamera->ShowWindow(SW_HIDE);
	m_DialogCamera->InitDialog();

	//��ʾinput����
	CRect rect_input;
	GetDlgItem(IDC_PICTURE_CALI)->GetWindowRect(&rect_input);
	ScreenToClient(&rect_input);
	m_DialogInput = new DialogInput();
	m_DialogInput->Create(IDD_DIALOG_INPUT, this);  //���Ի���Ϊdisplay��
	m_DialogInput->MoveWindow(rect_input.left, rect_input.top, rect_input.Width(), rect_input.Height());
	m_DialogInput->ShowWindow(SW_HIDE);
	m_DialogInput->InitDialog();

	//��ʾcalibration����
	CRect rect_calibration;
	GetDlgItem(IDC_PICTURE_CALI)->GetWindowRect(&rect_calibration);
	ScreenToClient(&rect_calibration);
	m_DialogCalibration = new DialogCalibration();
	m_DialogCalibration->Create(IDD_DIALOG_CALIBRATION, this);  //���Ի���Ϊdisplay��
	m_DialogCalibration->MoveWindow(rect_calibration.left, rect_calibration.top, rect_calibration.Width(), rect_calibration.Height());
	m_DialogCalibration->ShowWindow(SW_HIDE);
	m_DialogCalibration->InitDialog();

	// about ����
	m_DialogAbout.Create(IDD_ABOUTBOX, this);

	//����ϵͳ�����ļ�
	this->LoadSadsConfigFile();

	//����PLCͨ�ſؼ�
//	m_pWorkingThread->getMxWrapper()->SetActUtlMx(&m_ActUtl_Mx);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSADSProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSADSProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()));

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSADSProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//ѡ��˵���-����-��Ϣѡ���ʾ��Ӧ�Ľ���
void CSADSProjectDlg::OnClickedMenuInterfaceMessage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_DialogDisplayA->ShowWindow(SW_SHOW);
	m_DialogDisplayB->ShowWindow(SW_SHOW);
	m_DialogDisplayC->ShowWindow(SW_SHOW);

	m_DialogCalibration->ShowWindow(SW_HIDE);
	m_DialogInput->ShowWindow(SW_HIDE);
	
	m_DialogCamera->ShowWindow(SW_HIDE);
	if (m_DialogCamera->GetIsVideoOpen() == true)
	{
		m_DialogCamera->CloseVideo();
	}
}

//ѡ��˵���-����-�궨ѡ���ʾ��Ӧ�Ľ���
void CSADSProjectDlg::OnClickedMenuInterfaceCalibration()
{
	if (!m_isNeedShowCali)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("δ���øù��ܣ���鿴ϵͳ�����ļ�"));
		return;
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_DialogDisplayA->ShowWindow(SW_HIDE);
	m_DialogDisplayB->ShowWindow(SW_SHOW);
	m_DialogDisplayC->ShowWindow(SW_SHOW);

	m_DialogCalibration->ShowWindow(SW_SHOW);
	m_DialogInput->ShowWindow(SW_HIDE);

	m_DialogCamera->ShowWindow(SW_HIDE);
	if (m_DialogCamera->GetIsVideoOpen() == true)
	{
		m_DialogCamera->CloseVideo();
	}
}

//ѡ��˵���-����-���ѡ���ʾ��Ӧ�Ľ���
void CSADSProjectDlg::OnClickedMenuInterfaceCamera()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_DialogDisplayA->ShowWindow(SW_HIDE);
	m_DialogDisplayB->ShowWindow(SW_HIDE);
	m_DialogDisplayC->ShowWindow(SW_SHOW);

	m_DialogCalibration->ShowWindow(SW_HIDE);
	m_DialogInput->ShowWindow(SW_HIDE);

	m_DialogCamera->ShowWindow(SW_SHOW);
	if (m_DialogCamera->GetIsVideoOpen() == true)
	{
		m_DialogCamera->CloseVideo();
	}
}

//ѡ��˵���-����-����ѡ���ʾ��Ӧ�Ľ���
void CSADSProjectDlg::OnClickedMenuInterfaceInput()
{
	if (!m_isNeedShowInput)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("δ���øù��ܣ���鿴ϵͳ�����ļ�"));
		return;
	}
	// TODO:  �ڴ���������������
	m_DialogDisplayA->ShowWindow(SW_HIDE);
	m_DialogDisplayB->ShowWindow(SW_SHOW);
	m_DialogDisplayC->ShowWindow(SW_SHOW);

	m_DialogCalibration->ShowWindow(SW_HIDE);
	m_DialogInput->ShowWindow(SW_SHOW);

	m_DialogCamera->ShowWindow(SW_HIDE);
	if (m_DialogCamera->GetIsVideoOpen() == true)
	{
		m_DialogCamera->CloseVideo();
	}
}


//ѡ��˵���-ϵͳ-����ѡ�ϵͳ���������г�ʼ��
void CSADSProjectDlg::OnMenuSysOpen()
{
	m_pWorkingThread->d2w_open();
}

void CSADSProjectDlg::OnMenuSysClose()
{
	// TODO:  �ڴ���������������
	if (IDYES == AfxMessageBox(TEXT("ȷ���ػ���"), MB_YESNO))
	{
		m_pWorkingThread->d2w_close();
	}
}

void CSADSProjectDlg::OnMenuSysExit()
{
	// TODO:  �ڴ���������������
	if (IDYES == AfxMessageBox(TEXT("ȷ���˳�������"), MB_YESNO))
	{
		m_pWorkingThread->d2w_exit();
		//�˳�����
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}

void CSADSProjectDlg::OnMenuSysStopauto()
{
	// TODO:  �ڴ���������������
	if (IDYES == AfxMessageBox(TEXT("ȷ���˳��Զ�ƴװ��"), MB_YESNO))
	{
		m_pWorkingThread->d2w_stopAuto();
	}
}

void CSADSProjectDlg::OnMenuOpenAutoCatch()
{
	// TODO:  �ڴ���������������
	AdjustThread::setD500_Copy(1);
}

bool CSADSProjectDlg::LoadSadsConfigFile()
{
	CString filePath = MessageProcess::GetFilePath(_T("ini\\SadsIni.xml")); //�ļ���
	using namespace std;
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// ����XML�ļ�
	if (!doc.LoadFile(path))
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("��ȡ���������ļ�ʧ�ܣ�����ԭ�������ļ���ʽ����·������"));
		return false;
	}

	//��ʼ����
	TiXmlElement* root = doc.RootElement();//����ǩ
	if (root == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�root��ǩ�쳣"));
		return false;
	}
	//cali����
	TiXmlElement* interElem = root->FirstChildElement("interface");
	if (interElem == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�<interface>��ǩ�쳣"));
		return false;
	}
	int CALI = 0;
	int errorCode = interElem->QueryIntAttribute("CALI", &CALI);
	if (errorCode == TIXML_WRONG_TYPE)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�CALI���������쳣��"));
		return false;
	}
	if (CALI == 0)
	{
		m_isNeedShowCali = false;
	}
	else
	{
		m_isNeedShowCali = true;
	}

	//input����
	int INPUT = 0;
	errorCode = interElem->QueryIntAttribute("INPUT", &INPUT);
	if (errorCode == TIXML_WRONG_TYPE)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("�����ļ�INPUT���������쳣��"));
		return false;
	}
	if (INPUT == 0)
	{
		m_isNeedShowInput = false;
	}
	else
	{
		m_isNeedShowInput = true;
	}

	//�������doc
	doc.Clear();
	return true;
}

void CSADSProjectDlg::OnMenuAbout()
{
	// TODO:  �ڴ���������������
	m_DialogAbout.ShowWindow(SW_SHOW);
}


