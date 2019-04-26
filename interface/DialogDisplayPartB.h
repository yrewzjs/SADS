#pragma once
// DialogDisplayPartB �Ի���

class DialogDisplayPartB : public CDialogEx
{
	DECLARE_DYNAMIC(DialogDisplayPartB)

public:
	DialogDisplayPartB(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogDisplayPartB();

// �Ի�������
	enum { IDD = IDD_DIALOG_DISPLAYPART_B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	CListCtrl m_ListSensor;  //��ʾ���������ݼ���Ϣ
	CListCtrl m_ListCamera;  //��ʾ������ݼ���Ϣ
	CListCtrl m_ListCylinder; //��ʾ�͸׶�����Ϣ
public:
	void InitDialog();   //��ʼ��dialog
	// ����Ϣ��ʾ���͸���Ϣ��
	void ShowCylinderMsg(const struct cylinder_struct& cylins);
	// ����Ϣ��ʾ�������Ϣ��
	void ShowLaserMsg(const struct ranging_struct& rangings);
	// ����Ϣ��ʾ��ͼ����Ϣ��
	void ShowCameraMsg(const struct coords_struct& coords);
	// ��plc��Ϣ��ʾ��plc��ʾ��
	void ShowPlcMsg(const struct rplc_struct& rplc);
	// �����ģ��״̬��Ϣ��ʾ����ʾ��
	void ShowLaserStateMsg(const enum ModuleState state);
	// ��ͼ��ģ��״̬��Ϣ��ʾ����ʾ��
	void ShowCameraStateMsg(const enum ModuleState state);
	// ��PLCģ��״̬��Ϣ��ʾ����ʾ��
	void ShowPlcStateMsg(const enum ModuleState state);
};
