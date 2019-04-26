// DialogDisplayPartC.cpp : ʵ���ļ�
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
	//����ָ��λͼ��Դ BmpͼƬID
	m_Bit4Show.LoadBitmap(IDB_BITMAP_ICON);
	//��ȡ�Ի����ϵľ�� ͼƬ�ؼ�ID
	CStatic *p = (CStatic *)GetDlgItem(IDC_PICTURE_INC_ICON);
	//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//��ͼƬ���õ�Picture�ؼ���
	p->SetBitmap(m_Bit4Show);
}

//��Ϣ��ʾ����ʾ��Ϣ
void DialogDisplayPartC::ShowEditDisplayMsg(const CString newMsg)
{
	//GetDlgItemText(IDC_EDIT_DISPLAY, strRecv);
	if (++m_MsgCount % 100 == 0)
	{
		m_DisplayMsg = _T(""); //100���ͷ�һ��
	}

	if (!m_DisplayMsg.IsEmpty())
	{
		m_DisplayMsg += "\r\n"; //����
	}
	//�����ʾ��Ϣ
	m_DisplayMsg += newMsg;
	//��ʾ
	SetDlgItemText(IDC_EDIT_INC_DISPLAY, m_DisplayMsg);
	//���û�����
	m_EditDisplay.LineScroll(m_EditDisplay.GetLineCount());

	//return CT2A(newMsg);
}
// DialogDisplayPartC ��Ϣ�������
