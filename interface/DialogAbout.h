#pragma once


// DialogAbout �Ի���

class DialogAbout : public CDialogEx
{
	DECLARE_DYNAMIC(DialogAbout)

public:
	DialogAbout(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogAbout();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
