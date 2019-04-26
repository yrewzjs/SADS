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
  @param:_msg ��Ϣ
  @param:id ������id
*/
string MessageProcess::ProcessMessage(const CString _msg, const unsigned int id){
	CString msg = _T("");
	msg.Format(_T("ID = %d��%s"), id, _msg);
	return ProcessMessage(msg);
}

string MessageProcess::ProcessMessage(const CString msg){

#ifdef DIALOG_EXIST 
	CSADSProjectDlg* m_pMainDialog = (CSADSProjectDlg*)theApp.m_pMainWnd;
	m_pMainDialog->m_DialogDisplayC->ShowEditDisplayMsg(msg);
#endif

	//�����������־
	string str = CT2A(msg);
	return str;
}

/*
@param:_msg ��Ϣ
@param:id ������id
*/
string MessageProcess::FormatLogMessage(const CString _msg, const unsigned int id){
	CString msg = _T("");
	msg.Format(_T("ID = %d��%s"), id, _msg);
	return FormatLogMessage(msg);
}

string MessageProcess::FormatLogMessage(const CString msg)
{
	//�����������־
	string str = CT2A(msg);
	return str;
}

/***
ϵͳ��־��ʼ�����ã�
1��������־�����ļ�
*/
void MessageProcess::InitLogConfiguration()
{
	/* ������־�����ļ� */
	//������־�����ļ�·��
	const CString logPath = GetFilePath(_T("ini\\LogConfig.conf"));
	const string logPathStr = CT2A(logPath);
	///* ������־�����ļ� */
	easyloggingpp::Configurations confFromFile(logPathStr);  // Load configuration from file
	easyloggingpp::Loggers::reconfigureAllLoggers(confFromFile); // Re-configures all the loggers to current configuration file
	easyloggingpp::Configurations defaultConf;
	defaultConf.setToDefault();
	easyloggingpp::Loggers::reconfigureLogger("business", defaultConf); // Only business logger uses default configurations
}

CString MessageProcess::GetFilePath(LPCTSTR const strCurFileName)
{
	// �õ���ǰexe���ļ���
	CString strFileName = GetExePath(FALSE);

	//��������·��
	CString strPath = _T("");
	strPath.Format(_T("%s%s"), strFileName, strCurFileName);

	return strPath;
}

CString MessageProcess::GetExePath(BOOL bIncludeSep)
{
	// �õ���ǰ���ļ���
	CString strFileName;
	//���ص�ǰ�����Ѽ��ؿ�ִ�л�DLL�ļ�������·����(��'\0'��ֹ)����ģ������ɵ�ǰ���̵�ַ�ռ���ء�
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();

	if (bIncludeSep)
	{
		return strFileName; //exe�ļ�·��
	}
	else
	{
		return strFileName = strFileName.Left(strFileName.ReverseFind(_T('\\')) + 1);//exe�ļ���·�����������"\\"
	}
}