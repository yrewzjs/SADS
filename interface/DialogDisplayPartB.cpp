// DialogDisplayPartB.cpp : 实现文件
//

#include "stdafx.h"
#include "SADSProject.h"
#include "DialogDisplayPartB.h"
#include "afxdialogex.h"

#include "coords_struct.h"
#include "MXComonentDef.h"
#include "LaserDef.h"
#include "SadsDef.h"
// DialogDisplayPartB 对话框

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
	/*       设置Sensor数据表格            */
	// 设置列表控件显示网格线
	m_ListSensor.SetExtendedStyle(LVS_EX_GRIDLINES);
	// 设置列宽不变
	CHeaderCtrl*   pHeaderCtrlSensor = (CHeaderCtrl*)m_ListSensor.GetHeaderCtrl();
	pHeaderCtrlSensor->EnableWindow(FALSE);
	//在列表控件中插入列
	m_ListSensor.InsertColumn(0, _T("传感器"), LVCFMT_LEFT, 80);
	m_ListSensor.InsertColumn(1, _T("标定值"), LVCFMT_LEFT, 80);
	m_ListSensor.InsertColumn(2, _T("测量值"), LVCFMT_LEFT, 80);
	m_ListSensor.InsertColumn(3, _T("真实值"), LVCFMT_LEFT, 80);

	//在列表控件中插入行
	const int sensorNum = 21;
	for (int m = 0; m < sensorNum; m++)
	{
		CString strItem = _T("");
		strItem.Format(_T("ID=%2d %s"), m, LaserDef::ParseSensorMark(m));
		m_ListSensor.InsertItem(m, strItem);
	}

	/*       设置Camera数据表格            */
	// 设置列表控件显示网格线
	m_ListCamera.SetExtendedStyle(LVS_EX_GRIDLINES);
	// 设置列宽不变
	CHeaderCtrl*   pHeaderCtrlCamera = (CHeaderCtrl*)m_ListCamera.GetHeaderCtrl();
	pHeaderCtrlCamera->EnableWindow(FALSE);
	//在列表控件中插入列
	m_ListCamera.InsertColumn(0, _T("相机"), LVCFMT_LEFT, 60);
	m_ListCamera.InsertColumn(1, _T("坐标一"), LVCFMT_LEFT, 80);
	m_ListCamera.InsertColumn(2, _T("坐标二"), LVCFMT_LEFT, 80);

	//在列表控件中插入行
	const int cameraNum = 10;
	for (int m = 0; m < cameraNum; m++)
	{
		CString strItem = _T("");
		strItem.Format(_T("ID=%2d"), m);
		m_ListCamera.InsertItem(m, strItem);
	}

	/*       设置Cylinder动作表格            */
	// 设置列表控件显示网格线
	m_ListCylinder.SetExtendedStyle(LVS_EX_GRIDLINES);
	// 设置列宽不变
	CHeaderCtrl*   pHeaderCtrlCylinder = (CHeaderCtrl*)m_ListCylinder.GetHeaderCtrl();
	pHeaderCtrlCylinder->EnableWindow(FALSE);
	//在列表控件中插入列
	m_ListCylinder.InsertColumn(0, _T("油缸"), LVCFMT_LEFT, 60);
	m_ListCylinder.InsertColumn(1, _T("动作"), LVCFMT_LEFT, 80);
	m_ListCylinder.InsertColumn(2, _T("位置"), LVCFMT_LEFT, 80);

	//在列表控件中插入行
	m_ListCylinder.InsertItem(0, _T("红"));
	m_ListCylinder.InsertItem(1, _T("蓝"));
	m_ListCylinder.InsertItem(2, _T("滑动"));
	m_ListCylinder.InsertItem(3, _T("旋转"));
	m_ListCylinder.InsertItem(4, _T("俯仰"));
	m_ListCylinder.InsertItem(5, _T("横摇"));

	//状态显示
	this->ShowLaserStateMsg(UNSTART);
	this->ShowCameraStateMsg(UNSTART);
	this->ShowPlcStateMsg(UNSTART);

}
//信息显示区显示消息
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
			m_ListCamera.SetItemText(i, j, strItem); //0~9为坐标一，10~20为坐标二
		}
	}
}

// 将信息显示到测距信息区
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


// 将信息显示到油缸信息区
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
				if (i == 3) //旋转
				{
					switch (cylins.cylin_action[i])
					{
					case 0:
						strItem = _T("保持");
						break;
					case 1:
						strItem = _T("左旋");
						break;
					case 2:
						strItem = _T("右旋");
						break;
					}
				}
				else
				{
					switch (cylins.cylin_action[i])
					{
					case 0:
						strItem = _T("保持");
						break;
					case 1:
						strItem = _T("伸出");
						break;
					case 2:
						strItem = _T("收缩");
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
PLC状态
0   未启动
1   正常运行
2   主PLC系统异常
3   自动化PLC异常
*/
// 将plc信息显示到显示区
void DialogDisplayPartB::ShowPlcMsg(const rplc_struct& rplc)
{
	const short int D500 = rplc.lpsData_D5xx[0];  // 精定位类型
	const short int D512 = rplc.lpsData_D5xx[12]; // PLC状态
	const short int D513 = rplc.lpsData_D5xx[13]; // PLC操作模式
	const short int D516 = rplc.lpsData_D5xx[16]; // 粗定位当前动作

	switch (D500)
	{
	case 0:
	{
			  SetDlgItemText(IDC_EDIT_PLC_REQ, _T("无请求"));
			  break;
	}
	case 1:
	{
			  SetDlgItemText(IDC_EDIT_PLC_REQ, _T("抓 取"));
			  break;
	}
	case 2:
	{
			  SetDlgItemText(IDC_EDIT_PLC_REQ, _T("拼 装"));
			  break;
	}
	}

	switch (D512)
	{
	case 0:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("未启动"));
			  break;
	}
	case 1:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("运行中"));
			  break;
	}
	case 2:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("主异常"));
			  break;
	}
	case 3:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("自异常"));
			  break;
	}
	}

	switch (D513)
	{
	case 0:
	{
			  SetDlgItemText(IDC_EDIT_PLC_STYLE, _T("人工拼装"));
			  break;
	}
	case 1:
	{
			  SetDlgItemText(IDC_EDIT_PLC_STYLE, _T("自动拼装"));
			  break;
	}
	}

	switch (D516)
	{
	case 0:
	{
			  SetDlgItemText(IDC_EDIT_PLC_ACTION, _T("位置初始化"));
			  break;
	}
	case 1:
	{
			  SetDlgItemText(IDC_EDIT_PLC_ACTION, _T("抓取 准备"));
			  break;
	}
	case 2:
	{
			  SetDlgItemText(IDC_EDIT_PLC_ACTION, _T("抓取 动作"));
			  break;
	}
	case 3:
	{
			  SetDlgItemText(IDC_EDIT_PLC_ACTION, _T("拼装 动作"));
			  break;
	}
	}
}

// 将测距模块状态信息显示到显示区
void DialogDisplayPartB::ShowLaserStateMsg(const ModuleState state)
{
	switch (state)
	{
	case UNSTART:
	{
			  //显示
			  SetDlgItemText(IDC_EDIT_STATE_LASER, _T("未启动"));
			  break;
	}
	case RUNNING:
	{
			  //显示
			  SetDlgItemText(IDC_EDIT_STATE_LASER, _T("运行中"));
			  break;
	}
	case EXCEPTION:
	{
			  //显示
			  SetDlgItemText(IDC_EDIT_STATE_LASER, _T("异 常"));
			  break;
	}
	}
}

// 将图像模块状态信息显示到显示区
void DialogDisplayPartB::ShowCameraStateMsg(const ModuleState state)
{
	switch (state)
	{
	case UNSTART:
	{
			  //显示
			  SetDlgItemText(IDC_EDIT_STATE_CAM, _T("未启动"));
			  break;
	}
	case RUNNING:
	{
			  //显示
			  SetDlgItemText(IDC_EDIT_STATE_CAM, _T("运行中"));
			  break;
	}
	case EXCEPTION:
	{
			  //显示
			  SetDlgItemText(IDC_EDIT_STATE_CAM, _T("异 常"));
			  break;
	}
	}
}

// 将测距模块状态信息显示到显示区
void DialogDisplayPartB::ShowPlcStateMsg(const ModuleState state)
{
	switch (state)
	{
	case UNSTART:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("未启动"));
			  break;
	}
	case RUNNING:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("运行中"));
			  break;
	}
	case EXCEPTION:
	{
			  SetDlgItemText(IDC_EDIT_STATE_PLC, _T("异 常"));
			  break;
	}
	}
}
// DialogDisplayPartB 消息处理程序
