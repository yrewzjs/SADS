#pragma once


// DialogAbout 对话框

class DialogAbout : public CDialogEx
{
	DECLARE_DYNAMIC(DialogAbout)

public:
	DialogAbout(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogAbout();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
