#include "stdafx.h"
#include <read_writeXML.h>
bool readSTcoords(coords_struct &CS)
{
	const CString fileName = _T("ini\\StCoordsIni.xml");  //文件名
	CString filePath = GetFilePath(fileName);
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// 读入XML文件
	if (!doc.LoadFile(path)){
		//MessageProcess::ProcessException(INIT_ERROR, _T("读取配置文件失败！\r\n可能原因：配置文件格式错误、路径错误等。"), 0);
		return false;
	}
	int errorCode;
	int xvalue;
	int yvalue;
	//int xID = 0; //xvalue ID 从0递增。
	//int yID = 0; //yvalue ID 从0递增。
	int squadID = 0;//squad ID 从0递增。
	int modeId = 0;//mode ID 从0递增。
	bool isfunction;

	//开始解析
	TiXmlElement* root = doc.RootElement();//根标签
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

	/***********    进入<mode>标签  *********************/

	for (modeElem; modeElem != 0; modeElem = modeElem->NextSiblingElement())
	{
		errorCode = modeElem->QueryIntAttribute("id", &modeId);

		if (errorCode == TIXML_WRONG_TYPE){
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<mode>标签id属性类型异常！"), 0);
			return false;
		}
		else if (modeId < 0 || modeId > 1){
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<mode>标签id属性数值错误！"), 0);
			return false;
		}
		else if (modeId == loadedModeId){
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<mode>标签id属性重复！"), 0);
			return false;
		}

		loadedModeId = modeId;//已载入Group id
		if (modeElem == NULL){
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<mode>标签异常！"), 0);
			return false;
		}

		int loadedSquadId = -1;

		/***********    进入<squad>标签  *********************/

		squadElem = modeElem->FirstChildElement();
		for (squadElem; squadElem != 0; squadElem = squadElem->NextSiblingElement())
		{
			if (squadElem == NULL)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<squad>标签异常！"), 0);
				return false;
			}

			errorCode = squadElem->QueryIntAttribute("id", &squadID);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件squad id类型异常！"), ipGroupId);
				return false;
			}
			else if (squadID<1 || squadID>6)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件squad id数值异常！"), port);
				return false;
			}
			else if (squadID == loadedSquadId)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<squad>标签id属性重复！"), 0);
				return false;
			}

			loadedSquadId = squadID;//已载入Group id
			if (squadElem == NULL)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<squad>标签异常！"), 0);
				return false;
			}

			errorCode = squadElem->QueryBoolAttribute("isfunction", &isfunction);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件squad id类型异常！"), ipGroupId);
				return false;
			}

			if (CS._group == squadID)
			{
				CS.isfunction = isfunction;


				int loadedCOORDid = -1;

				/***********    进入<coord>标签  *********************/

				int coordID = 0; //coord ID 从0递增。

				coordElem = squadElem->FirstChildElement();
				for (coordElem; coordElem != 0; coordElem = coordElem->NextSiblingElement())
				{
					if (coordElem == NULL)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<coord>标签异常！"), 0);
						return false;
					}

					errorCode = coordElem->QueryIntAttribute("id", &coordID);
					if (errorCode == TIXML_WRONG_TYPE)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件coord id类型异常！"), ipGroupId);
						return false;
					}
					else if (coordID<0 || coordID>19)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件coord id数值异常！"), port);
						return false;
					}
					else if (coordID == loadedCOORDid){
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<coord>标签id属性重复！"), 0);
						return false;
					}

					loadedCOORDid = coordID;//已载入Group id
					if (coordElem == NULL){
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<coord>标签异常！"), 0);
						return false;
					}

					//获得x坐标
					errorCode = coordElem->QueryIntAttribute("xvalue", &xvalue);
					if (errorCode == TIXML_WRONG_TYPE)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件xvalue类型异常！"), port);
						return false;
					}
					else if (xvalue<0 || xvalue>1920)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件xvalue数值异常！"), port);
						return false;
					}
					CS.c_x[coordID] = xvalue;

					//获得y坐标
					errorCode = coordElem->QueryIntAttribute("yvalue", &yvalue);
					if (errorCode == TIXML_WRONG_TYPE)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件yvalue类型异常！"), port);
						return false;
					}
					else if (yvalue<0 || yvalue>1920)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件yvalue数值异常！"), port);
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
	const CString fileName = _T("ini\\StCoordsIni.xml");  //文件名
	CString filePath = GetFilePath(fileName);
	string strStl = CT2A(filePath);
	const char *path = strStl.c_str();
	TiXmlDocument doc;
	// 读入XML文件
	if (!doc.LoadFile(path)){
		//MessageProcess::ProcessException(INIT_ERROR, _T("读取配置文件失败！\r\n可能原因：配置文件格式错误、路径错误等。"), 0);
		return false;
	}

	//开始解析
	TiXmlElement* root = doc.RootElement();//根标签
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

	/***********    进入<mode>标签  *********************/
	for (modeElem; modeElem != 0; modeElem = modeElem->NextSiblingElement())
	{
		/***********    进入<squad>标签  *********************/
		squadElem = modeElem->FirstChildElement();
		if (squadElem == NULL)
		{
			//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<squad>标签异常！"), 0);
			return false;
		}
		for (squadElem; squadElem != 0; squadElem = squadElem->NextSiblingElement())
		{
			errorCode = squadElem->QueryIntAttribute("id", &squadID);
			if (errorCode == TIXML_WRONG_TYPE)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件squad id类型异常！"), ipGroupId);
				return false;
			}
			else if (squadID<1 || squadID>6)
			{
				//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件squad id数值异常！"), port);
				return false;
			}
			if (CS._group == squadID)
			{
				squadElem->SetAttribute("isfunction", 1);

				int coordID;
				/***********    进入<coord>标签  *********************/
				coordElem = squadElem->FirstChildElement();
				if (coordElem == NULL)
				{
					//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<coord>标签异常！"), 0);
					return false;
				}
				for (coordElem; coordElem != 0; coordElem = coordElem->NextSiblingElement())
				{
					if (coordElem == NULL)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件<coord>标签异常！"), 0);
						return false;
					}
					errorCode = coordElem->QueryIntAttribute("id", &coordID);
					if (errorCode == TIXML_WRONG_TYPE)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件coord id类型异常！"), ipGroupId);
						return false;
					}
					else if (coordID<0 || coordID>19)
					{
						//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件coord id数值异常！"), port);
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
		//MessageProcess::ProcessException(INIT_ERROR, _T("配置文件保存失败！"), _id);
		return false;
	}
	//释放资源
	doc.Clear();
	return true;
}