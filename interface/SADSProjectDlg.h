
// SADSProjectDlg.h : 头文件
//

#ifndef _CSADSPROJECTDLG_H_
#define _CSADSPROJECTDLG_H_

#include "DialogAbout.h"
#include "CActUtlType.h"
// CSADSProjectDlg 对话框
class CSADSProjectDlg : public CDialogEx
{
// 构造
public:
	CSADSProjectDlg(CWnd* pParent = NULL);	// 标准构造函数
	
	~CSADSProjectDlg(); // 自定义析构函数

// 对话框数据
	enum { IDD = IDD_SADSPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	class DialogAbout m_DialogAbout; // 模式对话框
	class DialogCalibration* m_DialogCalibration; //标定界面,无模式对话框
	class DialogInput* m_DialogInput; //输入界面,无模式对话框

public:
	class DialogCamera* m_DialogCamera;    //图像显示界面,无模式对话框
	class DialogDisplayPartA* m_DialogDisplayA;  //显示界面A,无模式对话框
	class DialogDisplayPartB* m_DialogDisplayB;  //显示界面B,无模式对话框
	class DialogDisplayPartC* m_DialogDisplayC;  //显示界面C,无模式对话框

	class WorkingThread* m_pWorkingThread;
//	CActUtlType m_ActUtl_Mx;
// PLC通信控件


private:
	bool LoadSadsConfigFile();   // 解析程序配置文件
	bool m_isNeedShowCali;
	bool m_isNeedShowInput;
public:
	// 自定义的消息映射函数
	afx_msg void OnClickedMenuInterfaceMessage();
	afx_msg void OnClickedMenuInterfaceCalibration();
	afx_msg void OnClickedMenuInterfaceCamera();
	afx_msg void OnClickedMenuInterfaceInput();

	afx_msg void OnMenuSysOpen();
	afx_msg void OnMenuSysClose();
	afx_msg void OnMenuSysExit();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuSysStopauto();
	afx_msg void OnMenuOpenAutoCatch();
};

#endif