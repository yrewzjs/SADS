#pragma once
#include "afxwin.h"


// DialogDisplayPartC 对话框

class DialogDisplayPartC : public CDialogEx
{
	DECLARE_DYNAMIC(DialogDisplayPartC)

public:
	DialogDisplayPartC(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogDisplayPartC();

// 对话框数据
	enum { IDD = IDD_DIALOG_DISPLAYPART_C };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_EditDisplay;
	CString m_DisplayMsg; //存储m_EditDisplay显示的信息
	unsigned int m_MsgCount;  //当前m_EditDisplay显示的信息条数,界面上可以同时显示40条

private:
	CBitmap m_Bit4Show;
	CStatic m_Picture_Icon;

public:
	void InitDialog();
	// 将信息显示到信息显示区
	void ShowEditDisplayMsg(const CString newMsg);

};
