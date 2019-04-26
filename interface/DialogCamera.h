#pragma once
#include "afxwin.h"

//#include <atlimage.h>

// DialogCamera �Ի���

class DialogCamera : public CDialogEx
{
	DECLARE_DYNAMIC(DialogCamera)

public:
	DialogCamera(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogCamera();

// �Ի�������
	enum { IDD = IDD_DIALOG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCameraSingle();
	afx_msg void OnBnClickedButtonCameraContinue();
	afx_msg void OnCbnSelchangeCamNum();
	//afx_msg BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	void InitDialog();
	void ShowImage(CImage *image);
	bool GetIsVideoOpen();
	//void SetIsVideoOpen(bool isopen);
	void CloseVideo();
private:
	CStatic m_CameraTitle;  // ����
	CFont m_Font;  // ��������
	int m_ComboCamNum;  // ��ѡ�����
	bool m_isVideoOpen;  // ��Ƶģʽ�Ƿ���
};
