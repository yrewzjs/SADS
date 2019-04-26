#include "stdafx.h"
#include "MessageProcess.h"

#define DIALOG_EXIST  1555
#ifdef  DIALOG_EXIST 
	#include "SADSProject.h"
	#include "SADSProjectDlg.h"
	#include "DialogDisplayPartC.h"
#endif

#ifndef _INIT_LOG_
	_INITIALIZE_EASYLOGGINGPP
#define _INIT_LOG_
#endif
/*
  @param:_msg 信息
  @param:id 传感器id
*/
string MessageProcess::ProcessMessage(const CString _msg, const unsigned int id){
	CString msg = _T("");
	msg.Format(_T("ID = %d，%s"), id, _msg);
	return ProcessMessage(msg);
}

string MessageProcess::ProcessMessage(const CString msg){

#ifdef DIALOG_EXIST 
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogDisplayC->ShowEditDisplayMsg(msg);
#endif

	//返回输出到日志
	string str = CT2A(msg);
	return str;
}

/*
@param:_msg 信息
@param:id 传感器id
*/
string MessageProcess::FormatLogMessage(const CString _msg, const unsigned int id){
	CString msg = _T("");
	msg.Format(_T("ID = %d，%s"), id, _msg);
	return FormatLogMessage(msg);
}

string MessageProcess::FormatLogMessage(const CString msg)
{
	//返回输出到日志
	string str = CT2A(msg);
	return str;
}

/***
系统日志初始化设置：
1、载入日志配置文件
*/
void MessageProcess::InitLogConfiguration()
{
	/* 载入日志配置文件 */
	//构建日志配置文件路径
	const CString logPath = GetFilePath(_T("ini\\LogConfig.conf"));
	const string logPathStr = CT2A(logPath);
	///* 载入日志配置文件 */
	easyloggingpp::Configurations confFromFile(logPathStr);  // Load configuration from file
	easyloggingpp::Loggers::reconfigureAllLoggers(confFromFile); // Re-configures all the loggers to current configuration file
	easyloggingpp::Configurations defaultConf;
	defaultConf.setToDefault();
	easyloggingpp::Loggers::reconfigureLogger("business", defaultConf); // Only business logger uses default configurations
}

CString MessageProcess::GetFilePath(LPCTSTR const strCurFileName)
{
	// 得到当前exe的文件名
	CString strFileName = GetExePath(FALSE);

	//构建完整路径
	CString strPath = _T("");
	strPath.Format(_T("%s%s"), strFileName, strCurFileName);

	return strPath;
}

CString MessageProcess::GetExePath(BOOL bIncludeSep)
{
	// 得到当前的文件名
	CString strFileName;
	//返回当前进程已加载可执行或DLL文件的完整路径名(以'\0'终止)，该模块必须由当前进程地址空间加载。
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();

	if (bIncludeSep)
	{
		return strFileName; //exe文件路径
	}
	else
	{
		return strFileName = strFileName.Left(strFileName.ReverseFind(_T('\\')) + 1);//exe文件夹路径，包括最后"\\"
	}
}