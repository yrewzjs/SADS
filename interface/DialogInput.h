#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DialogInput 对话框

class DialogInput : public CDialogEx
{
	DECLARE_DYNAMIC(DialogInput)

public:
	DialogInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogInput();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	void InitDialog();   //自定义初始化dialog

public:
	int m_Edit_InputKLoc;  //K管片位置  -18000~18000
	int m_ComboInputCurSeg;  // 当前管片
	int m_ComboInputLastSeg; // 喂片机末端管片
	int m_ComboFlagsD2060;  // D2060标志位

	int m_Radio_Yg;  // 油缸选择组
	int m_Radio_Level;  // 速度级别选择组
	int m_Radio_Dir;  // 方向选择组
	int m_EditInputVelocity;  // 速度输入

public:
	afx_msg void OnBnClickedButtonKloc();
	afx_msg void OnBnClickedButtonCurseg();
	afx_msg void OnBnClickedButtonLastseg();
	afx_msg void OnBnClickedButtonInput2060();
	afx_msg void OnBnClickedButtonInputSd();

	afx_msg void OnRadioCylinderClicked();
	afx_msg void OnRadioLevelClicked();
	afx_msg void OnRadioDireClicked();

	afx_msg void OnChangeKLoc();
	afx_msg void OnChangeVelocity();

	afx_msg void OnComboSelChangeFlagD2060();
	afx_msg void OnComboSelChangeCurSeg();
	afx_msg void OnComboSelChangeLastSeg();
};
