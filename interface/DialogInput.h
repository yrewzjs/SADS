#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DialogInput �Ի���

class DialogInput : public CDialogEx
{
	DECLARE_DYNAMIC(DialogInput)

public:
	DialogInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogInput();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	void InitDialog();   //�Զ����ʼ��dialog

public:
	int m_Edit_InputKLoc;  //K��Ƭλ��  -18000~18000
	int m_ComboInputCurSeg;  // ��ǰ��Ƭ
	int m_ComboInputLastSeg; // ιƬ��ĩ�˹�Ƭ
	int m_ComboFlagsD2060;  // D2060��־λ

	int m_Radio_Yg;  // �͸�ѡ����
	int m_Radio_Level;  // �ٶȼ���ѡ����
	int m_Radio_Dir;  // ����ѡ����
	int m_EditInputVelocity;  // �ٶ�����

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
