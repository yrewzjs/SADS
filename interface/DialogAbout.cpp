// DialogAbout.cpp : 实现文件
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogAbout.h"
#include "afxdialogex.h"


// DialogAbout 对话框

IMPLEMENT_DYNAMIC(DialogAbout, CDialogEx)

DialogAbout::DialogAbout(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogAbout::IDD, pParent)
{

}

DialogAbout::~DialogAbout()
{
}

void DialogAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogAbout, CDialogEx)
END_MESSAGE_MAP()


// DialogAbout 消息处理程序
