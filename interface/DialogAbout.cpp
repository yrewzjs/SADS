// DialogAbout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogAbout.h"
#include "afxdialogex.h"


// DialogAbout �Ի���

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


// DialogAbout ��Ϣ�������
