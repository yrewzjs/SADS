// DialogDisplayPartC.cpp : 实现文件
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogDisplayPartC.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(DialogDisplayPartC, CDialogEx)

DialogDisplayPartC::DialogDisplayPartC(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogDisplayPartC::IDD, pParent)
{
}

DialogDisplayPartC::~DialogDisplayPartC()
{
}

void DialogDisplayPartC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INC_DISPLAY, m_EditDisplay);
	DDX_Control(pDX, IDC_PICTURE_INC_ICON, m_Picture_Icon);
}


BEGIN_MESSAGE_MAP(DialogDisplayPartC, CDialogEx)
END_MESSAGE_MAP()


void DialogDisplayPartC::InitDialog()
{
	//加载指定位图资源 Bmp图片ID
	m_Bit4Show.LoadBitmap(IDB_BITMAP_ICON);
	//获取对话框上的句柄 图片控件ID
	CStatic *p = (CStatic *)GetDlgItem(IDC_PICTURE_INC_ICON);
	//设置静态控件窗口风格为位图居中显示
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//将图片设置到Picture控件上
	p->SetBitmap(m_Bit4Show);
}

//信息显示区显示消息
void DialogDisplayPartC::ShowEditDisplayMsg(const CString newMsg)
{
	//GetDlgItemText(IDC_EDIT_DISPLAY, strRecv);
	if (++m_MsgCount % 100 == 0)
	{
		m_DisplayMsg = _T(""); //100条释放一次
	}

	if (!m_DisplayMsg.IsEmpty())
	{
		m_DisplayMsg += "\r\n"; //换行
	}
	//添加显示信息
	m_DisplayMsg += newMsg;
	//显示
	SetDlgItemText(IDC_EDIT_INC_DISPLAY, m_DisplayMsg);
	//设置滑动框
	m_EditDisplay.LineScroll(m_EditDisplay.GetLineCount());

	//return CT2A(newMsg);
}
// DialogDisplayPartC 消息处理程序
