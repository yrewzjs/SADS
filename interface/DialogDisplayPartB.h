#pragma once
// DialogDisplayPartB 对话框

class DialogDisplayPartB : public CDialogEx
{
	DECLARE_DYNAMIC(DialogDisplayPartB)

public:
	DialogDisplayPartB(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogDisplayPartB();

// 对话框数据
	enum { IDD = IDD_DIALOG_DISPLAYPART_B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


private:
	CListCtrl m_ListSensor;  //显示传感器数据及信息
	CListCtrl m_ListCamera;  //显示相机数据及信息
	CListCtrl m_ListCylinder; //显示油缸动作信息
public:
	void InitDialog();   //初始化dialog
	// 将信息显示到油缸信息区
	void ShowCylinderMsg(const struct cylinder_struct& cylins);
	// 将信息显示到测距信息区
	void ShowLaserMsg(const struct ranging_struct& rangings);
	// 将信息显示到图像信息区
	void ShowCameraMsg(const struct coords_struct& coords);
	// 将plc信息显示到plc显示区
	void ShowPlcMsg(const struct rplc_struct& rplc);
	// 将测距模块状态信息显示到显示区
	void ShowLaserStateMsg(const enum ModuleState state);
	// 将图像模块状态信息显示到显示区
	void ShowCameraStateMsg(const enum ModuleState state);
	// 将PLC模块状态信息显示到显示区
	void ShowPlcStateMsg(const enum ModuleState state);
};
