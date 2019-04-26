#pragma once
#include "afxwin.h"

// DialogCalibration 对话框

class DialogCalibration : public CDialogEx
{
	DECLARE_DYNAMIC(DialogCalibration)

public:
	DialogCalibration(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogCalibration();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void InitDialog();

private:
	int m_ComboCaliPlc;   // PLC标定选项1下拉框
	int m_EditCaliPlc;

	int m_Radio_Group1;
	int m_Radio_Group2;
	int m_Radio_Group3;
	int m_Radio_Group4;

	int m_ComboLaserCommend;  // 激光标定下拉框
	int m_ComboCamCommend;    // 图像标定下拉框

public:
	afx_msg void OnBnClickedButtonCaliLaser();
	afx_msg void OnBnClickedButtonCaliCamera();
	afx_msg void OnBnClickedButtonPlc1();
	afx_msg void OnBnClickedButtonPlc2();
	afx_msg void OnBnClickedButtonPlc3();
	afx_msg void OnBnClickedButtonPlc4();
	afx_msg void OnBnClickedButtonPlc5();

	afx_msg void OnRadioGroup1Clicked();
	afx_msg void OnRadioGroup2Clicked();
	afx_msg void OnRadioGroup3Clicked();
	afx_msg void OnRadioGroup4Clicked();

	afx_msg void OnCbnSelChangeLaserCommend();
	afx_msg void OnCbnSelChangeCamCommend();
	
	afx_msg void OnCbnSelChangeCaliPlc();
	afx_msg void OnEnChangeEditCaliPlc();
};
