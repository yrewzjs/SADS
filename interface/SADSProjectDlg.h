
// SADSProjectDlg.h : ͷ�ļ�
//

#ifndef _CSADSPROJECTDLG_H_
#define _CSADSPROJECTDLG_H_

#include "DialogAbout.h"
#include "CActUtlType.h"
// CSADSProjectDlg �Ի���
class CSADSProjectDlg : public CDialogEx
{
// ����
public:
	CSADSProjectDlg(CWnd* pParent = NULL);	// ��׼���캯��
	
	~CSADSProjectDlg(); // �Զ�����������

// �Ի�������
	enum { IDD = IDD_SADSPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	class DialogAbout m_DialogAbout; // ģʽ�Ի���
	class DialogCalibration* m_DialogCalibration; //�궨����,��ģʽ�Ի���
	class DialogInput* m_DialogInput; //�������,��ģʽ�Ի���

public:
	class DialogCamera* m_DialogCamera;    //ͼ����ʾ����,��ģʽ�Ի���
	class DialogDisplayPartA* m_DialogDisplayA;  //��ʾ����A,��ģʽ�Ի���
	class DialogDisplayPartB* m_DialogDisplayB;  //��ʾ����B,��ģʽ�Ի���
	class DialogDisplayPartC* m_DialogDisplayC;  //��ʾ����C,��ģʽ�Ի���

	class WorkingThread* m_pWorkingThread;
//	CActUtlType m_ActUtl_Mx;
// PLCͨ�ſؼ�


private:
	bool LoadSadsConfigFile();   // �������������ļ�
	bool m_isNeedShowCali;
	bool m_isNeedShowInput;
public:
	// �Զ������Ϣӳ�亯��
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