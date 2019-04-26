#pragma once


// DialogDisplayPartA �Ի���

class DialogDisplayPartA : public CDialogEx
{
	DECLARE_DYNAMIC(DialogDisplayPartA)

public:
	DialogDisplayPartA(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogDisplayPartA();

// �Ի�������
	enum { IDD = IDD_DIALOG_DISPLAYPART_A };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	HICON m_IconCircle_Blue;
	HICON m_IconCircle_Red;
	HICON m_IconCircle_Grey;
	HICON m_IconCircle_Green;

	// �ƽ��͸���ʾ
	CStatic m_PictrueCircle_1;
	CStatic m_PictrueCircle_2;
	CStatic m_PictrueCircle_3;
	CStatic m_PictrueCircle_4;
	CStatic m_PictrueCircle_5;
	CStatic m_PictrueCircle_6;
	CStatic m_PictrueCircle_7;
	CStatic m_PictrueCircle_8;
	CStatic m_PictrueCircle_9;
	CStatic m_PictrueCircle_10;
	CStatic m_PictrueCircle_11;
	CStatic m_PictrueCircle_12;
	CStatic m_PictrueCircle_13;
	CStatic m_PictrueCircle_14;
	CStatic m_PictrueCircle_15;
	CStatic m_PictrueCircle_16;
	CStatic m_PictrueCircle_17;
	CStatic m_PictrueCircle_18;
	CStatic m_PictrueCircle_19;
	//��Ƭ��ʾ
	HICON m_IconRect_Blue;
	HICON m_IconRect_Green;
	HICON m_IconRect_Grey;
	HICON m_IconRect_Red;
	CStatic m_PictureSegment_B1;
	CStatic m_PictureSegment_B2;
	CStatic m_PictureSegment_B3;
	CStatic m_PictureSegment_B4;
	CStatic m_PictureSegment_L1;
	CStatic m_PictureSegment_L2;
	CStatic m_PictureSegment_F;

public:
	void InitDialog();
	// ����Ϣ��ʾ����Ƭƴװ��Ϣ��
	void ShowSegmentMsg(const struct segment_struct &segments);
	// ����Ϣ��ʾ���ƽ��͸���
	void ShowThrustCylinderMsg(const struct thrust_cylinder_struct &cylins);
};
