#include "stdafx.h"
#include <read_writeXML.h>
bool readSTcoords(coords_struct &CS)
{
	const CString fileName = _T("ini\\StCoordsIni.xml");  //�ļ���
	CString filePath = GetFilePath(fileName);
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// ����XML�ļ�
	if (!doc.LoadFile(path)){
		//MessageProcess::ProcessException(INIT_ERROR, _T("��ȡ�����ļ�ʧ�ܣ�\r\n����ԭ�������ļ���ʽ����·������ȡ�"), 0);
		return false;
	}
	int errorCode;
	int xvalue;
	int yvalue;
	//int xID = 0; //xvalue ID ��0������
	//int yID = 0; //yvalue ID ��0������
	int squadID = 0;//squad ID ��0������
	int modeId = 0;//mode ID ��0������
	bool isfunction;

	//��ʼ����
	TiXmlElement* root = doc.RootElement();//����ǩ
	if (root == NULL)
	{
		return false;
	}
	TiXmlElement* modeElem = root->FirstChildElement();
	if (modeElem == NULL)
	{
		return false;
	}

	TiXmlElement* squadElem = NULL;
	TiXmlElement* coordElem = NULL;
	//TiXmlElement* yElem = NULL;
	//TiXmlAttribute *attr = NULL;

	int loadedModeId = -1;

	/***********    ����<mode>��ǩ  *********************/

	for (modeElem; modeElem != 0; modeElem = modeElem->NextSiblingElement())
	{
		errorCode = modeElem->QueryIntAttribute("id", &modeId);

		if (errorCode == TIXML_WRONG_TYPE){
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<mode>��ǩid���������쳣��"), 0);
			return false;
		}
		else if (modeId < 0 || modeId > 1){
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<mode>��ǩid������ֵ����"), 0);
			return false;
		}
		else if (modeId == loadedModeId){
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<mode>��ǩid�����ظ���"), 0);
			return false;
		}

		loadedModeId = modeId;//������Group id
		if (modeElem == NULL){
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<mode>��ǩ�쳣��"), 0);
			return false;
		}

		int loadedSquadId = -1;

		/***********    ����<squad>��ǩ  *********************/

		squadElem = modeElem->FirstChildElement();
		for (squadElem; squadElem != 0; squadElem = squadElem->NextSiblingElement())
		{
			if (squadElem == NULL)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<squad>��ǩ�쳣��"), 0);
				return false;
			}

			errorCode = squadElem->QueryIntAttribute("id", &squadID);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�squad id�����쳣��"), ipGroupId);
				return false;
			}
			else if (squadID<1 || squadID>6)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�squad id��ֵ�쳣��"), port);
				return false;
			}
			else if (squadID == loadedSquadId)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<squad>��ǩid�����ظ���"), 0);
				return false;
			}

			loadedSquadId = squadID;//������Group id
			if (squadElem == NULL)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<squad>��ǩ�쳣��"), 0);
				return false;
			}

			errorCode = squadElem->QueryBoolAttribute("isfunction", &isfunction);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�squad id�����쳣��"), ipGroupId);
				return false;
			}

			if (CS._group == squadID)
			{
				CS.isfunction = isfunction;


				int loadedCOORDid = -1;

				/***********    ����<coord>��ǩ  *********************/

				int coordID = 0; //coord ID ��0������

				coordElem = squadElem->FirstChildElement();
				for (coordElem; coordElem != 0; coordElem = coordElem->NextSiblingElement())
				{
					if (coordElem == NULL)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<coord>��ǩ�쳣��"), 0);
						return false;
					}

					errorCode = coordElem->QueryIntAttribute("id", &coordID);
					if (errorCode == TIXML_WRONG_TYPE)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�coord id�����쳣��"), ipGroupId);
						return false;
					}
					else if (coordID<0 || coordID>19)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�coord id��ֵ�쳣��"), port);
						return false;
					}
					else if (coordID == loadedCOORDid){
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<coord>��ǩid�����ظ���"), 0);
						return false;
					}

					loadedCOORDid = coordID;//������Group id
					if (coordElem == NULL){
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<coord>��ǩ�쳣��"), 0);
						return false;
					}

					//���x����
					errorCode = coordElem->QueryIntAttribute("xvalue", &xvalue);
					if (errorCode == TIXML_WRONG_TYPE)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�xvalue�����쳣��"), port);
						return false;
					}
					else if (xvalue<0 || xvalue>1920)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�xvalue��ֵ�쳣��"), port);
						return false;
					}
					CS.c_x[coordID] = xvalue;

					//���y����
					errorCode = coordElem->QueryIntAttribute("yvalue", &yvalue);
					if (errorCode == TIXML_WRONG_TYPE)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�yvalue�����쳣��"), port);
						return false;
					}
					else if (yvalue<0 || yvalue>1920)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�yvalue��ֵ�쳣��"), port);
						return false;
					}
					CS.c_y[coordID] = yvalue;
				}
			}
		}
	}
	doc.Clear();
	return true;
}
bool writeSTcoords(coords_struct &CS)
{
	const CString fileName = _T("ini\\StCoordsIni.xml");  //�ļ���
	CString filePath = GetFilePath(fileName);
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// ����XML�ļ�
	if (!doc.LoadFile(path)){
		//MessageProcess::ProcessException(INIT_ERROR, _T("��ȡ�����ļ�ʧ�ܣ�\r\n����ԭ�������ļ���ʽ����·������ȡ�"), 0);
		return false;
	}

	//��ʼ����
	TiXmlElement* root = doc.RootElement();//����ǩ
	if (root == NULL)
	{
		return false;
	}
	TiXmlElement* modeElem = root->FirstChildElement();
	if (modeElem == NULL)
	{
		return false;
	}
	TiXmlElement* squadElem = NULL;
	TiXmlElement* coordElem = NULL;
	TiXmlElement* childElem = NULL;
	TiXmlAttribute *attr = NULL;
	int squadID;
	int errorCode;

	/***********    ����<mode>��ǩ  *********************/
	for (modeElem; modeElem != 0; modeElem = modeElem->NextSiblingElement())
	{
		/***********    ����<squad>��ǩ  *********************/
		squadElem = modeElem->FirstChildElement();
		if (squadElem == NULL)
		{
			//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<squad>��ǩ�쳣��"), 0);
			return false;
		}
		for (squadElem; squadElem != 0; squadElem = squadElem->NextSiblingElement())
		{
			errorCode = squadElem->QueryIntAttribute("id", &squadID);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�squad id�����쳣��"), ipGroupId);
				return false;
			}
			else if (squadID<1 || squadID>6)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�squad id��ֵ�쳣��"), port);
				return false;
			}
			if (CS._group == squadID)
			{
				squadElem->SetAttribute("isfunction", 1);

				int coordID;
				/***********    ����<coord>��ǩ  *********************/
				coordElem = squadElem->FirstChildElement();
				if (coordElem == NULL)
				{
					//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<coord>��ǩ�쳣��"), 0);
					return false;
				}
				for (coordElem; coordElem != 0; coordElem = coordElem->NextSiblingElement())
				{
					if (coordElem == NULL)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�<coord>��ǩ�쳣��"), 0);
						return false;
					}
					errorCode = coordElem->QueryIntAttribute("id", &coordID);
					if (errorCode == TIXML_WRONG_TYPE)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�coord id�����쳣��"), ipGroupId);
						return false;
					}
					else if (coordID<0 || coordID>19)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�coord id��ֵ�쳣��"), port);
						return false;
					}
					coordElem->SetAttribute("xvalue", CS.c_x[coordID]);
					coordElem->SetAttribute("yvalue", CS.c_y[coordID]);
				}
			}
		}
	}
	bool iswritten = doc.SaveFile(path);
	if (!iswritten){
		//MessageProcess::ProcessException(INIT_ERROR, _T("�����ļ�����ʧ�ܣ�"), _id);
		return false;
	}
	//�ͷ���Դ
	doc.Clear();
	return true;
}