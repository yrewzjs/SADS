#pragma once
#include "afxwin.h"

//#include <atlimage.h>

// DialogCamera 对话框

class DialogCamera : public CDialogEx
{
	DECLARE_DYNAMIC(DialogCamera)

public:
	DialogCamera(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogCamera();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	CStatic m_CameraTitle;  // 标题
	CFont m_Font;  // 标题字体
	int m_ComboCamNum;  // 所选择相机
	bool m_isVideoOpen;  // 视频模式是否开启
};
