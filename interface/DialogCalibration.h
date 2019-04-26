#pragma once
#include "afxwin.h"

// DialogCalibration �Ի���

class DialogCalibration : public CDialogEx
{
	DECLARE_DYNAMIC(DialogCalibration)

public:
	DialogCalibration(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogCalibration();

// �Ի�������
	enum { IDD = IDD_DIALOG_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void InitDialog();

private:
	int m_ComboCaliPlc;   // PLC�궨ѡ��1������
	int m_EditCaliPlc;

	int m_Radio_Group1;
	int m_Radio_Group2;
	int m_Radio_Group3;
	int m_Radio_Group4;

	int m_ComboLaserCommend;  // ����궨������
	int m_ComboCamCommend;    // ͼ��궨������

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
