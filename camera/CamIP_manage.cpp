#include "stdafx.h"
#include <CamIP_manage.h>

CString GetExePath(BOOL bIncludeSep)
{
	// �õ���ǰ���ļ���
	CString strFileName;
	//���ص�ǰ�����Ѽ��ؿ�ִ�л�DLL�ļ�������·����(��'\0'��ֹ)����ģ������ɵ�ǰ���̵�ַ�ռ���ء�
	//GetModuleFileName(AfxGetInstanceHandle(), strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();

	if (bIncludeSep)
		return strFileName; //exe�ļ�·��
	else{
		return strFileName = strFileName.Left(strFileName.ReverseFind(_T('\\')) + 1);//exe�ļ���·�����������"\\"
	}
}

CString GetFilePath(LPCTSTR const strCurFileName)
{
	// �õ���ǰexe���ļ���
	CString strFileName = GetExePath(FALSE);

	//��������·��
	CString strPath = _T("");
	strPath.Format(_T("%s%s"), strFileName, strCurFileName);

	return strPath;
}

bool readcamID(CamIP &myCamIP)
{
	const CString fileName = _T("ini\\CamaraModuleini.xml");  //�ļ���
	CString filePath = GetFilePath(fileName);
	std::string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// ����XML�ļ�
	if (!doc.LoadFile(path)){
		//MessageProcess::ProcessException(INIT_ERROR, _T("��ȡ�����ļ�ʧ�ܣ�\r\n����ԭ�������ļ���ʽ����·������ȡ�"), 0);
		return false;
	}

	int errorCode;
	int port;
	int triID1;
	int triID2;
	bool conn;
	CString ip;
	int camaraId = 0; //camara ID ��0������
	int groupId = 0;//Group ID ��0������
	//��ʼ����
	TiXmlElement* root = doc.RootElement();//����ǩ
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

	/***********    ����<group>��ǩ  *********************/

	for (groupElem; groupElem != 0; groupElem = groupElem->NextSiblingElement())
	{
		errorCode = groupElem->QueryIntAttribute("id", &groupId);

		if (errorCode == TIXML_WRONG_TYPE){
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<group>��ǩid���������쳣��"), 0);
			return false;
		}
		else if (groupId < 0 || groupId > 1){
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<group>��ǩid������ֵ����"), 0);
			return false;
		}
		else if (groupId == loadedGroupId){
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<group>��ǩid�����ظ���"), 0);
			return false;
		}

		loadedGroupId = groupId;//������Group id
		if (groupElem == NULL){
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<group>��ǩ�쳣��"), 0);
			return false;
		}

		int loadedCamaraID = -1;

		/***********    ����<camara>��ǩ  *********************/

		CaElem = groupElem->FirstChildElement();
		for (CaElem; CaElem != 0; CaElem = CaElem->NextSiblingElement())
		{
			if (CaElem == NULL)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<camara>��ǩ�쳣��"), 0);
				return false;
			}

			errorCode = CaElem->QueryIntAttribute("id", &camaraId);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�camara id�����쳣��"), ipGroupId);
				return false;
			}
			else if (camaraId<0 || camaraId>9)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�camara id��ֵ�쳣��"), port);
				return false;
			}
			else if (camaraId == loadedCamaraID){
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<camara>��ǩid�����ظ���"), 0);
				return false;
			}

			loadedCamaraID = camaraId;//������Group id
			if (CaElem == NULL){
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<camara>��ǩ�쳣��"), 0);
				return false;
			}

			//ȡ��ip
			attr = CaElem->LastAttribute();
			myCamIP.cam_ip[camaraId] = attr->Value();

			//��ȡport
			errorCode = CaElem->QueryIntAttribute("port", &port);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ��˿������쳣��"), port);
				return false;
			}
			else if (port<1024)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ��˿���ֵ�쳣��"), port);
				return false;
			}
			myCamIP.cam_port[camaraId] = port;

			//��ȡ���Ǳ���
			errorCode = CaElem->QueryIntAttribute("triID1", &triID1);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ��˿������쳣��"), port);
				return false;
			}
			else if (triID1<0 || triID1>9)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ��˿���ֵ�쳣��"), port);
				return false;
			}
			myCamIP.tri_num[camaraId][0] = triID1;

			errorCode = CaElem->QueryIntAttribute("triID2", &triID2);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ��˿������쳣��"), port);
				return false;
			}
			else if (triID2<10 || triID2>19)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ��˿���ֵ�쳣��"), port);
				return false;
			}
			myCamIP.tri_num[camaraId][1] = triID2;

			//��ȡ�Ƿ���Ҫ����
			errorCode = CaElem->QueryBoolAttribute("conn", &conn);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ��˿������쳣��"), port);
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


	//��ȡxml�ļ��и������Ӧ�ı�ţ�IP���˿�
	bool isOK = readcamID(c_CamIP);

	return isOK;
}

CamIP CamIPg::out_cam()
{
	return c_CamIP;
}