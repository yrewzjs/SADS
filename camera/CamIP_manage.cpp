#include "stdafx.h"
#include <CamIP_manage.h>

CString GetExePath(BOOL bIncludeSep)
{
	// 得到当前的文件名
	CString strFileName;
	//返回当前进程已加载可执行或DLL文件的完整路径名(以'\0'终止)，该模块必须由当前进程地址空间加载。
	//GetModuleFileName(AfxGetInstanceHandle(), strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();

	if (bIncludeSep)
		return strFileName; //exe文件路径
	else{
		return strFileName = strFileName.Left(strFileName.ReverseFind(_T('\\')) + 1);//exe文件夹路径，包括最后"\\"
	}
}

CString GetFilePath(LPCTSTR const strCurFileName)
{
	// 得到当前exe的文件名
	CString strFileName = GetExePath(FALSE);

	//构建完整路径
	CString strPath = _T("");
	strPath.Format(_T("%s%s"), strFileName, strCurFileName);

	return strPath;
}

bool readcamID(CamIP &myCamIP)
{
	const CString fileName = _T("ini\\CamaraModuleini.xml");  //文件名
	CString filePath = GetFilePath(fileName);
	std::string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// 读入XML文件
	if (!doc.LoadFile(path)){
		//MessageProcess::ProcessException(INIT_ERROR, _T("读取配置文件失败！\r\n可能原因：配置文件格式错误、路径错误等。"), 0);
		return false;
	}

	int errorCode;
	int port;
	int triID1;
	int triID2;
	bool conn;
	CString ip;
	int camaraId = 0; //camara ID 从0递增。
	int groupId = 0;//Group ID 从0递增。
	//开始解析
	TiXmlElement* root = doc.RootElement();//根标签
	if (root == NULL)
	{
		return false;
	}
	TiXmlElement* groupElem = root->FirstChildElement();
	if (groupElem == NULL)
	{
		return false;
	}

	TiXmlElement* CaElem = NULL;
	TiXmlAttribute *attr = NULL;

	int loadedGroupId = -1;

	/***********    进入<group>标签  *********************/

	for (groupElem; groupElem != 0; groupElem = groupElem->NextSiblingElement())
	{
		errorCode = groupElem->QueryIntAttribute("id", &groupId);

		if (errorCode == TIXML_WRONG_TYPE){
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<group>标签id属性类型异常！"), 0);
			return false;
		}
		else if (groupId < 0 || groupId > 1){
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<group>标签id属性数值错误！"), 0);
			return false;
		}
		else if (groupId == loadedGroupId){
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<group>标签id属性重复！"), 0);
			return false;
		}

		loadedGroupId = groupId;//已载入Group id
		if (groupElem == NULL){
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<group>标签异常！"), 0);
			return false;
		}

		int loadedCamaraID = -1;

		/***********    进入<camara>标签  *********************/

		CaElem = groupElem->FirstChildElement();
		for (CaElem; CaElem != 0; CaElem = CaElem->NextSiblingElement())
		{
			if (CaElem == NULL)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<camara>标签异常！"), 0);
				return false;
			}

			errorCode = CaElem->QueryIntAttribute("id", &camaraId);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件camara id类型异常！"), ipGroupId);
				return false;
			}
			else if (camaraId<0 || camaraId>9)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件camara id数值异常！"), port);
				return false;
			}
			else if (camaraId == loadedCamaraID){
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<camara>标签id属性重复！"), 0);
				return false;
			}

			loadedCamaraID = camaraId;//已载入Group id
			if (CaElem == NULL){
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<camara>标签异常！"), 0);
				return false;
			}

			//取出ip
			attr = CaElem->LastAttribute();
			myCamIP.cam_ip[camaraId] = attr->Value();

			//获取port
			errorCode = CaElem->QueryIntAttribute("port", &port);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件端口类型异常！"), port);
				return false;
			}
			else if (port<1024)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件端口数值异常！"), port);
				return false;
			}
			myCamIP.cam_port[camaraId] = port;

			//获取三角标编号
			errorCode = CaElem->QueryIntAttribute("triID1", &triID1);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件端口类型异常！"), port);
				return false;
			}
			else if (triID1<0 || triID1>9)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件端口数值异常！"), port);
				return false;
			}
			myCamIP.tri_num[camaraId][0] = triID1;

			errorCode = CaElem->QueryIntAttribute("triID2", &triID2);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件端口类型异常！"), port);
				return false;
			}
			else if (triID2<10 || triID2>19)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件端口数值异常！"), port);
				return false;
			}
			myCamIP.tri_num[camaraId][1] = triID2;

			//获取是否需要连接
			errorCode = CaElem->QueryBoolAttribute("conn", &conn);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件端口类型异常！"), port);
				return false;
			}
			myCamIP.conn[camaraId] = conn;
		}
	}
	doc.Clear();
	return true;
}

CamIPg::CamIPg()
{
	c_CamIP = { { 0 }, { 0 }, { 0 }, { 0 }, { { 0 } } };
}
CamIPg::~CamIPg()
{

}
bool CamIPg::get_cam()
{

	/*c_CamIP.conn[1] = 1;
	////c_CamIP.num[1] = 0;
	//c_CamIP.cam_ip[1] = "192.168.1.60";
	//c_CamIP.cam_port[1] = 2017;
	//c_CamIP.tri_num[1][0] = 1;
	c_CamIP.tri_num[1][1] = 1+10;*/


	//读取xml文件中各相机对应的编号，IP，端口
	bool isOK = readcamID(c_CamIP);

	return isOK;
}

CamIP CamIPg::out_cam()
{
	return c_CamIP;
}