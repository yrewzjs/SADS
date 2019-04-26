#pragma once
#include "afxwin.h"


// DialogDisplayPartC �Ի���

class DialogDisplayPartC : public CDialogEx
{
	DECLARE_DYNAMIC(DialogDisplayPartC)

public:
	DialogDisplayPartC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogDisplayPartC();

// �Ի�������
	enum { IDD = IDD_DIALOG_DISPLAYPART_C };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_EditDisplay;
	CString m_DisplayMsg; //�洢m_EditDisplay��ʾ����Ϣ
	unsigned int m_MsgCount;  //��ǰm_EditDisplay��ʾ����Ϣ����,�����Ͽ���ͬʱ��ʾ40��

private:
	CBitmap m_Bit4Show;
	CStatic m_Picture_Icon;

public:
	void InitDialog();
	// ����Ϣ��ʾ����Ϣ��ʾ��
	void ShowEditDisplayMsg(const CString newMsg);

};
