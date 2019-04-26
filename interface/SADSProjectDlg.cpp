
// SADSProjectDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSADSProjectDlg 对话框



CSADSProjectDlg::CSADSProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSADSProjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//自定义
	//初始化日志组件
	MessageProcess::InitLogConfiguration();
	//开启working线程，该线程只会在主对话框初始化时启动，在程序退出时退出
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

	//自定义的事件响应映射

	// 第一个参数为菜单项ID,第二个参数为事件响应函数名称;
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


// CSADSProjectDlg 消息处理程序

BOOL CSADSProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//关闭按钮失效
	CMenu *pMenu = this->GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);     //关闭按钮变灰
	pMenu->ModifyMenu(SC_CLOSE,MF_BYCOMMAND | MF_GRAYED);  // 关闭按钮会无效
	// 显示主dialog
	ShowWindow(SW_NORMAL);

	//显示displayA界面
	CRect rect_displayA;
	GetDlgItem(IDC_PICTURE_PART_A)->GetWindowRect(&rect_displayA);
	ScreenToClient(&rect_displayA);
	m_DialogDisplayA = new DialogDisplayPartA();
	m_DialogDisplayA->Create(IDD_DIALOG_DISPLAYPART_A, this);
	m_DialogDisplayA->MoveWindow(rect_displayA.left, rect_displayA.top, rect_displayA.Width(), rect_displayA.Height());
	m_DialogDisplayA->ShowWindow(SW_SHOW);
	m_DialogDisplayA->InitDialog();

	//显示displayB界面
	CRect rect_displayB;
	GetDlgItem(IDC_PICTURE_PART_B)->GetWindowRect(&rect_displayB);
	ScreenToClient(&rect_displayB);
	m_DialogDisplayB = new DialogDisplayPartB();
	m_DialogDisplayB->Create(IDD_DIALOG_DISPLAYPART_B, this);
	m_DialogDisplayB->MoveWindow(rect_displayB.left, rect_displayB.top, rect_displayB.Width(), rect_displayB.Height());
	m_DialogDisplayB->ShowWindow(SW_SHOW);
	m_DialogDisplayB->InitDialog();

	//显示displayC界面
	CRect rect_displayC;
	GetDlgItem(IDC_PICTURE_PART_C)->GetWindowRect(&rect_displayC);
	ScreenToClient(&rect_displayC);
	m_DialogDisplayC = new DialogDisplayPartC();
	m_DialogDisplayC->Create(IDD_DIALOG_DISPLAYPART_C, this);
	m_DialogDisplayC->MoveWindow(rect_displayC.left, rect_displayC.top, rect_displayC.Width(), rect_displayC.Height());
	m_DialogDisplayC->ShowWindow(SW_SHOW);
	m_DialogDisplayC->InitDialog();
	
	//显示camera界面
	CRect rect_camera;
	GetDlgItem(IDC_PICTRUE_MAIN_CAMERA)->GetWindowRect(&rect_camera);
	ScreenToClient(&rect_camera);
	m_DialogCamera = new DialogCamera();
	m_DialogCamera->Create(IDD_DIALOG_CAMERA, this);
	m_DialogCamera->MoveWindow(rect_camera.left, rect_camera.top, rect_camera.Width(), rect_camera.Height());
	m_DialogCamera->ShowWindow(SW_HIDE);
	m_DialogCamera->InitDialog();

	//显示input界面
	CRect rect_input;
	GetDlgItem(IDC_PICTURE_CALI)->GetWindowRect(&rect_input);
	ScreenToClient(&rect_input);
	m_DialogInput = new DialogInput();
	m_DialogInput->Create(IDD_DIALOG_INPUT, this);  //父对话框为display框
	m_DialogInput->MoveWindow(rect_input.left, rect_input.top, rect_input.Width(), rect_input.Height());
	m_DialogInput->ShowWindow(SW_HIDE);
	m_DialogInput->InitDialog();

	//显示calibration界面
	CRect rect_calibration;
	GetDlgItem(IDC_PICTURE_CALI)->GetWindowRect(&rect_calibration);
	ScreenToClient(&rect_calibration);
	m_DialogCalibration = new DialogCalibration();
	m_DialogCalibration->Create(IDD_DIALOG_CALIBRATION, this);  //父对话框为display框
	m_DialogCalibration->MoveWindow(rect_calibration.left, rect_calibration.top, rect_calibration.Width(), rect_calibration.Height());
	m_DialogCalibration->ShowWindow(SW_HIDE);
	m_DialogCalibration->InitDialog();

	// about 界面
	m_DialogAbout.Create(IDD_ABOUTBOX, this);

	//载入系统配置文件
	this->LoadSadsConfigFile();

	//传递PLC通信控件
//	m_pWorkingThread->getMxWrapper()->SetActUtlMx(&m_ActUtl_Mx);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSADSProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()));

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSADSProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//选择菜单栏-界面-信息选项，显示相应的界面
void CSADSProjectDlg::OnClickedMenuInterfaceMessage()
{
	// TODO:  在此添加控件通知处理程序代码
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

//选择菜单栏-界面-标定选项，显示相应的界面
void CSADSProjectDlg::OnClickedMenuInterfaceCalibration()
{
	if (!m_isNeedShowCali)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("未启用该功能，请查看系统配置文件"));
		return;
	}
	// TODO:  在此添加控件通知处理程序代码
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

//选择菜单栏-界面-相机选项，显示相应的界面
void CSADSProjectDlg::OnClickedMenuInterfaceCamera()
{
	// TODO:  在此添加控件通知处理程序代码
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

//选择菜单栏-界面-输入选项，显示相应的界面
void CSADSProjectDlg::OnClickedMenuInterfaceInput()
{
	if (!m_isNeedShowInput)
	{
		LOG(INFO) << MessageProcess::ProcessMessage(_T("未启用该功能，请查看系统配置文件"));
		return;
	}
	// TODO:  在此添加命令处理程序代码
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


//选择菜单栏-系统-开机选项，系统开机并进行初始化
void CSADSProjectDlg::OnMenuSysOpen()
{
	m_pWorkingThread->d2w_open();
}

void CSADSProjectDlg::OnMenuSysClose()
{
	// TODO:  在此添加命令处理程序代码
	if (IDYES == AfxMessageBox(TEXT("确定关机吗？"), MB_YESNO))
	{
		m_pWorkingThread->d2w_close();
	}
}

void CSADSProjectDlg::OnMenuSysExit()
{
	// TODO:  在此添加命令处理程序代码
	if (IDYES == AfxMessageBox(TEXT("确定退出程序吗？"), MB_YESNO))
	{
		m_pWorkingThread->d2w_exit();
		//退出程序
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}

void CSADSProjectDlg::OnMenuSysStopauto()
{
	// TODO:  在此添加命令处理程序代码
	if (IDYES == AfxMessageBox(TEXT("确定退出自动拼装吗？"), MB_YESNO))
	{
		m_pWorkingThread->d2w_stopAuto();
	}
}

void CSADSProjectDlg::OnMenuOpenAutoCatch()
{
	// TODO:  在此添加命令处理程序代码
	AdjustThread::setD500_Copy(1);
}

bool CSADSProjectDlg::LoadSadsConfigFile()
{
	CString filePath = MessageProcess::GetFilePath(_T("ini\\SadsIni.xml")); //文件名
	using namespace std;
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// 读入XML文件
	if (!doc.LoadFile(path))
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("读取程序配置文件失败，可能原因：配置文件格式错误、路径错误"));
		return false;
	}

	//开始解析
	TiXmlElement* root = doc.RootElement();//根标签
	if (root == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件root标签异常"));
		return false;
	}
	//cali界面
	TiXmlElement* interElem = root->FirstChildElement("interface");
	if (interElem == NULL)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件<interface>标签异常"));
		return false;
	}
	int CALI = 0;
	int errorCode = interElem->QueryIntAttribute("CALI", &CALI);
	if (errorCode == TIXML_WRONG_TYPE)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件CALI数据属性异常！"));
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

	//input界面
	int INPUT = 0;
	errorCode = interElem->QueryIntAttribute("INPUT", &INPUT);
	if (errorCode == TIXML_WRONG_TYPE)
	{
		LOG(ERROR) << MessageProcess::ProcessMessage(_T("配置文件INPUT数据属性异常！"));
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

	//读完清除doc
	doc.Clear();
	return true;
}

void CSADSProjectDlg::OnMenuAbout()
{
	// TODO:  在此添加命令处理程序代码
	m_DialogAbout.ShowWindow(SW_SHOW);
}


