// DialogDisplayPartA.cpp : 实现文件
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogDisplayPartA.h"
#include "afxdialogex.h"

#include "SadsDef.h"

// DialogDisplayPartA 对话框

IMPLEMENT_DYNAMIC(DialogDisplayPartA, CDialogEx)

DialogDisplayPartA::DialogDisplayPartA(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogDisplayPartA::IDD, pParent)
{
	m_IconCircle_Blue = AfxGetApp()->LoadIcon(IDI_ICON_BLUE_CIRCLE);
	m_IconCircle_Red = AfxGetApp()->LoadIcon(IDI_ICON_RED_CIRCLE);
	m_IconCircle_Grey = AfxGetApp()->LoadIcon(IDI_ICON_GREY_CIRCLE);
	m_IconCircle_Green = AfxGetApp()->LoadIcon(IDI_ICON_GREEN_CIRCLE);

	m_IconRect_Blue = AfxGetApp()->LoadIcon(IDI_ICON_BLUE_RECT);
	m_IconRect_Red = AfxGetApp()->LoadIcon(IDI_ICON_RED_RECT);
	m_IconRect_Grey = AfxGetApp()->LoadIcon(IDI_ICON_GREY_RECT);
	m_IconRect_Green = AfxGetApp()->LoadIcon(IDI_ICON_GREEN_RECT);
}

DialogDisplayPartA::~DialogDisplayPartA()
{
}

void DialogDisplayPartA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_1, m_PictrueCircle_1);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_2, m_PictrueCircle_2);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_3, m_PictrueCircle_3);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_4, m_PictrueCircle_4);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_5, m_PictrueCircle_5);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_6, m_PictrueCircle_6);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_7, m_PictrueCircle_7);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_8, m_PictrueCircle_8);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_9, m_PictrueCircle_9);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE1_E0, m_PictrueCircle_10);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_11, m_PictrueCircle_11);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_12, m_PictrueCircle_12);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_13, m_PictrueCircle_13);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_14, m_PictrueCircle_14);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_15, m_PictrueCircle_15);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_16, m_PictrueCircle_16);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_17, m_PictrueCircle_17);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_18, m_PictrueCircle_18);
	DDX_Control(pDX, IDC_PICTURE_CIRCLE_19, m_PictrueCircle_19);
	DDX_Control(pDX, IDC_PICTURE_SEGMENT_B1, m_PictureSegment_B1);
	DDX_Control(pDX, IDC_PICTURE_SEGMENT_B2, m_PictureSegment_B2);
	DDX_Control(pDX, IDC_PICTURE_SEGMENT_B3, m_PictureSegment_B3);
	DDX_Control(pDX, IDC_PICTURE_SEGMENT_B4, m_PictureSegment_B4);
	DDX_Control(pDX, IDC_PICTURE_SEGMENT_L1, m_PictureSegment_L1);
	DDX_Control(pDX, IDC_PICTURE_SEGMENT_L2, m_PictureSegment_L2);
	DDX_Control(pDX, IDC_PICTURE_SEGMENT_F, m_PictureSegment_F);
}


BEGIN_MESSAGE_MAP(DialogDisplayPartA, CDialogEx)
END_MESSAGE_MAP()


void DialogDisplayPartA::InitDialog()
{
	/*   显示  */
	m_PictrueCircle_1.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_2.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_3.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_4.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_5.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_6.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_7.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_8.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_9.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_10.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_11.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_12.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_13.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_14.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_15.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_16.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_17.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_18.SetIcon(m_IconCircle_Grey);
	m_PictrueCircle_19.SetIcon(m_IconCircle_Grey);
	//显示管片状态
	m_PictureSegment_B1.SetIcon(m_IconRect_Grey);
	m_PictureSegment_B2.SetIcon(m_IconRect_Grey);
	m_PictureSegment_B3.SetIcon(m_IconRect_Grey);
	m_PictureSegment_B4.SetIcon(m_IconRect_Grey);
	m_PictureSegment_L1.SetIcon(m_IconRect_Grey);
	m_PictureSegment_L2.SetIcon(m_IconRect_Grey);
	m_PictureSegment_F.SetIcon(m_IconRect_Grey);
}



// 将信息显示到管片拼装信息区
void DialogDisplayPartA::ShowSegmentMsg(const segment_struct &segments)
{
	HICON iconRect;
	for (int i = 0; i < SEGMENT_NUMBER; i++)
	{
		if (segments.segment_state[i] == UN_SPLICE)
			iconRect = m_IconRect_Grey;
		else if (segments.segment_state[i] ==SPLICING)
			iconRect = m_IconRect_Blue;
		else if (segments.segment_state[i] == SPLICE_FIN)
			iconRect = m_IconRect_Green;
		else if (segments.segment_state[i] == SPLICE_FAILED)
			iconRect = m_IconRect_Red;

		switch (i)
		{
		case 0:
			m_PictureSegment_B3.SetIcon(iconRect);
			break;
		case 1:
			m_PictureSegment_B2.SetIcon(iconRect);
			break;
		case 2:
			m_PictureSegment_B4.SetIcon(iconRect);
			break;
		case 3:
			m_PictureSegment_B1.SetIcon(iconRect);
			break;
		case 4:
			m_PictureSegment_L2.SetIcon(iconRect);
			break;
		case 5:
			m_PictureSegment_L1.SetIcon(iconRect);
			break;
		case 6:
			m_PictureSegment_F.SetIcon(iconRect);
			break;
		}
	}
}

// 将信息显示到推进油缸区
void DialogDisplayPartA::ShowThrustCylinderMsg(const thrust_cylinder_struct &cylins)
{
	HICON iconCircle;
	for (int i = 0; i < THRUST_CYLINDER_NUMBER; i++)
	{
		if (cylins.cylin_action[i] == 0)
			//油缸状态为“停”
			iconCircle = m_IconCircle_Grey;
		else if (cylins.cylin_action[i] == 1)
			//油缸状态为“伸”
			iconCircle = m_IconCircle_Blue;
		else if (cylins.cylin_action[i] == 2)
			//油缸状态为“缩”
			iconCircle = m_IconCircle_Red;
		else if (cylins.cylin_action[i] == 3)
			//油缸到达缩限位
			iconCircle = m_IconCircle_Green;

		switch (i)
		{
		case 0:
			m_PictrueCircle_1.SetIcon(iconCircle);
			break;
		case 1:
			m_PictrueCircle_2.SetIcon(iconCircle);
			break;
		case 2:
			m_PictrueCircle_3.SetIcon(iconCircle);
			break;
		case 3:
			m_PictrueCircle_4.SetIcon(iconCircle);
			break;
		case 4:
			m_PictrueCircle_5.SetIcon(iconCircle);
			break;
		case 5:
			m_PictrueCircle_6.SetIcon(iconCircle);
			break;
		case 6:
			m_PictrueCircle_7.SetIcon(iconCircle);
			break;
		case 7:
			m_PictrueCircle_8.SetIcon(iconCircle);
			break;
		case 8:
			m_PictrueCircle_9.SetIcon(iconCircle);
			break;
		case 9:
			m_PictrueCircle_10.SetIcon(iconCircle);
			break;
		case 10:
			m_PictrueCircle_11.SetIcon(iconCircle);
			break;
		case 11:
			m_PictrueCircle_12.SetIcon(iconCircle);
			break;
		case 12:
			m_PictrueCircle_13.SetIcon(iconCircle);
			break;
		case 13:
			m_PictrueCircle_14.SetIcon(iconCircle);
			break;
		case 14:
			m_PictrueCircle_15.SetIcon(iconCircle);
			break;
		case 15:
			m_PictrueCircle_16.SetIcon(iconCircle);
			break;
		case 16:
			m_PictrueCircle_17.SetIcon(iconCircle);
			break;
		case 17:
			m_PictrueCircle_18.SetIcon(iconCircle);
			break;
		case 18:
			m_PictrueCircle_19.SetIcon(iconCircle);
			break;

		}
	}
}

// DialogDisplayPartA 消息处理程序
