// DialogDisplayPartB.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogDisplayPartB.h"
#include "afxdialogex.h"

#include "coords_struct.h"
#include "MXComonentDef.h"
#include "LaserDef.h"
#include "SadsDef.h"
// DialogDisplayPartB �Ի���

IMPLEMENT_DYNAMIC(DialogDisplayPartB, CDialogEx)

DialogDisplayPartB::DialogDisplayPartB(CWnd* pParent /*=NULL*/)
: CDialogEx(DialogDisplayPartB::IDD, pParent)
{

}

DialogDisplayPartB::~DialogDisplayPartB()
{
}

void DialogDisplayPartB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SENSOR, m_ListSensor);
	DDX_Control(pDX, IDC_LIST_CAMERA, m_ListCamera);
	DDX_Control(pDX, IDC_LIST_CYLINDER, m_ListCylinder);
}


BEGIN_MESSAGE_MAP(DialogDisplayPartB, CDialogEx)
END_MESSAGE_MAP()


void DialogDisplayPartB::InitDialog()
{
	/*       ����Sensor���ݱ��            */
	// �����б�ؼ���ʾ������
	m_ListSensor.SetExtendedStyle(LVS_EX_GRIDLINES);
	// �����п���
	CHeaderCtrl*   pHeaderCtrlSensor = (CHeaderCtrl*)m_ListSensor.GetHeaderCtrl();
	pHeaderCtrlSensor->EnableWindow(FALSE);
	//���б�ؼ��в�����
	m_ListSensor.InsertColumn(0, _T("������"), LVCFMT_LEFT, 80);
	m_ListSensor.InsertColumn(1, _T("�궨ֵ"), LVCFMT_LEFT, 80);
	m_ListSensor.InsertColumn(2, _T("����ֵ"), LVCFMT_LEFT, 80);
	m_ListSensor.InsertColumn(3, _T("��ʵֵ"), LVCFMT_LEFT, 80);

	//���б�ؼ��в�����
	const int sensorNum = 21;
	for (int m = 0; m < sensorNum; m++)
	{
		CString strItem = _T("");
		strItem.Format(_T("ID=%2d %s"), m, LaserDef::ParseSensorMark(m));
		m_ListSensor.InsertItem(m, strItem);
	}

	/*       ����Camera���ݱ��            */
	// �����б�ؼ���ʾ������
	m_ListCamera.SetExtendedStyle(LVS_EX_GRIDLINES);
	// �����п���
	CHeaderCtrl*   pHeaderCtrlCamera = (CHeaderCtrl*)m_ListCamera.GetHeaderCtrl();
	pHeaderCtrlCamera->EnableWindow(FALSE);
	//���б�ؼ��в�����
	m_ListCamera.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60);
	m_ListCamera.InsertColumn(1, _T("����һ"), LVCFMT_LEFT, 80);
	m_ListCamera.InsertColumn(2, _T("�����"), LVCFMT_LEFT, 80);

	//���б�ؼ��в�����
	const int cameraNum = 10;
	for (int m = 0; m < cameraNum; m++)
	{
		CString strItem = _T("");
		strItem.Format(_T("ID=%2d"), m);
		m_ListCamera.InsertItem(m, strItem);
	}

	/*       ����Cylinder�������            */
	// �����б�ؼ���ʾ������
	m_ListCylinder.SetExtendedStyle(LVS_EX_GRIDLINES);
	// �����п���
	CHeaderCtrl*   pHeaderCtrlCylinder = (CHeaderCtrl*)m_ListCylinder.GetHeaderCtrl();
	pHeaderCtrlCylinder->EnableWindow(FALSE);
	//���б�ؼ��в�����
	m_ListCylinder.InsertColumn(0, _T("�͸�"), LVCFMT_LEFT, 60);
	m_ListCylinder.InsertColumn(1, _T("����"), LVCFMT_LEFT, 80);
	m_ListCylinder.InsertColumn(2, _T("λ��"), LVCFMT_LEFT, 80);

	//���б�ؼ��в�����
	m_ListCylinder.InsertItem(0, _T("��"));
	m_ListCylinder.InsertItem(1, _T("��"));
	m_ListCylinder.InsertItem(2, _T("����"));
	m_ListCylinder.InsertItem(3, _T("��ת"));
	m_ListCylinder.InsertItem(4, _T("����"));
	m_ListCylinder.InsertItem(5, _T("��ҡ"));

	//״̬��ʾ
	this->ShowLaserStateMsg(UNSTART);
	this->ShowCameraStateMsg(UNSTART);
	this->ShowPlcStateMsg(UNSTART);

}
//��Ϣ��ʾ����ʾ��Ϣ
void DialogDisplayPartB::ShowCameraMsg(const coords_struct& coords)
{
	const int cameraNum = 10;
	const int columns = 3;
	for (int i = 0; i < cameraNum; i++)
	{
		for (int j = 1; j < columns; j++)
		{
			int x_sub = (j - 1) * 10 + i;
			int y_sub = (j - 1) * 10 + i;
			CString strItem = _T("");
			strItem.Format(_T("%2d,%2d"), coords.c_x[x_sub], coords.c_y[y_sub]);
			m_ListCamera.SetItemText(i, j, strItem); //0~9Ϊ����һ��10~20Ϊ�����
		}
	}
}

// ����Ϣ��ʾ�������Ϣ��
void DialogDisplayPartB::ShowLaserMsg(const ranging_struct& rangings)
{
	const int columns = 4;
	for (int i = 0; i < SENSOR_NUMBER; i++)
	{
		for (int j = 1; j < columns; j++)
		{
			CString strItem = _T("");
			switch (j)
			{
			case 1:
				strItem.Format(_T("%.2f"), rangings.cali_v[i]);
				break;
			case 2:
				strItem.Format(_T("%.2f"), rangings.meas_v[i]);
				break;
			case 3:
				strItem.Format(_T("%.2f"), rangings.real_v[i]);
				break;

			}
			m_ListSensor.SetItemText(i, j, strItem);
		}
	}
}


// ����Ϣ��ʾ���͸���Ϣ��
void DialogDisplayPartB::ShowCylinderMsg(const cylinder_struct& cylins)
{
	CString strItem = _T("");
	const int columns = 3;
	for (int i = 0; i < CYLINDER_NUMBER; i++)
	{
		for (int j = 1; j < columns; j++)
		{
			CString strItem = _T("");
			if (j == 1)
			{
				if (i == 3) //��ת
				{
					switch (cylins.cylin_action[i])
					{
					case 0:
						strItem = _T("����");
						break;
					case 1:
						strItem = _T("����");
						break;
					case 2:
						strItem = _T("����");
						break;
					}
				}
				else
				{
					switch (cylins.cylin_action[i])
					{
					case 0:
						strItem = _T("����");
						break;
					case 1:
						strItem = _T("���");
						break;
					case 2:
						strItem = _T("����");
						break;
					}
				}
			}
			else
			{
				strItem.Format(_T("%.2f"), cylins.cylin_loc[i]);
			}
			m_ListCylinder.SetItemText(i, j, strItem);
		}
	}
}


/**
PLC״̬
0   δ����
1   ��������
2   ��PLCϵͳ�쳣
3   �Զ���PLC�쳣
*/
// ��plc��Ϣ��ʾ����ʾ��
void DialogDisplayPartB::ShowPlcMsg(const rplc_struct& rplc)
{
	const short int D500 = rplc.lpsData_D5xx[0];  // ����λ����
	const short int D512 = rplc.lpsData_D5xx[12]; // PLC״̬
	const short int D513 = rplc.lpsData_D5xx[13]; // PLC����ģʽ
	const short int D516 = rplc.lpsData_D5xx[16]; // �ֶ�λ��ǰ����

	switch (D500)
	{
	case 0:
	{
			  SetDlgItemText(IDC_EDIT_PLC_REQ, _T("������"));
			  break;
	}
	case 1:
	{
			  SetDlgItemText(IDC_EDIT_PLC_REQ, _T("ץ ȡ"));
			  break;
	}
	case 2:
	{
			  SetDlgItemText(IDC_EDIT_PLC_REQ, _T("ƴ װ"));
			  break;
	}
	}

	switch (D512)
	{
	case 0:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("δ����"));
			  break;
	}
	case 1:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("������"));
			  break;
	}
	case 2:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("���쳣"));
			  break;
	}
	case 3:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("���쳣"));
			  break;
	}
	}

	switch (D513)
	{
	case 0:
	{
			  SetDlgItemText(IDC_EDIT_PLC_STYLE, _T("�˹�ƴװ"));
			  break;
	}
	case 1:
	{
			  SetDlgItemText(IDC_EDIT_PLC_STYLE, _T("�Զ�ƴװ"));
			  break;
	}
	}

	switch (D516)
	{
	case 0:
	{
			  SetDlgItemText(IDC_EDIT_PLC_ACTION, _T("λ�ó�ʼ��"));
			  break;
	}
	case 1:
	{
			  SetDlgItemText(IDC_EDIT_PLC_ACTION, _T("ץȡ ׼��"));
			  break;
	}
	case 2:
	{
			  SetDlgItemText(IDC_EDIT_PLC_ACTION, _T("ץȡ ����"));
			  break;
	}
	case 3:
	{
			  SetDlgItemText(IDC_EDIT_PLC_ACTION, _T("ƴװ ����"));
			  break;
	}
	}
}

// �����ģ��״̬��Ϣ��ʾ����ʾ��
void DialogDisplayPartB::ShowLaserStateMsg(const ModuleState state)
{
	switch (state)
	{
	case UNSTART:
	{
			  //��ʾ
			  SetDlgItemText(IDC_EDIT_STATE_LASER, _T("δ����"));
			  break;
	}
	case RUNNING:
	{
			  //��ʾ
			  SetDlgItemText(IDC_EDIT_STATE_LASER, _T("������"));
			  break;
	}
	case EXCEPTION:
	{
			  //��ʾ
			  SetDlgItemText(IDC_EDIT_STATE_LASER, _T("�� ��"));
			  break;
	}
	}
}

// ��ͼ��ģ��״̬��Ϣ��ʾ����ʾ��
void DialogDisplayPartB::ShowCameraStateMsg(const ModuleState state)
{
	switch (state)
	{
	case UNSTART:
	{
			  //��ʾ
			  SetDlgItemText(IDC_EDIT_STATE_CAM, _T("δ����"));
			  break;
	}
	case RUNNING:
	{
			  //��ʾ
			  SetDlgItemText(IDC_EDIT_STATE_CAM, _T("������"));
			  break;
	}
	case EXCEPTION:
	{
			  //��ʾ
			  SetDlgItemText(IDC_EDIT_STATE_CAM, _T("�� ��"));
			  break;
	}
	}
}

// �����ģ��״̬��Ϣ��ʾ����ʾ��
void DialogDisplayPartB::ShowPlcStateMsg(const ModuleState state)
{
	switch (state)
	{
	case UNSTART:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("δ����"));
			  break;
	}
	case RUNNING:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("������"));
			  break;
	}
	case EXCEPTION:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("�� ��"));
			  break;
	}
	}
}
// DialogDisplayPartB ��Ϣ�������
