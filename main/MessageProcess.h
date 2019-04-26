#ifndef _MESSAGEPROCESS_H_
#define _MESSAGEPROCESS_H_
/**
* ������һ����Ϣ�����ࡣ
* @author zjs
* @Time 2017
*/

/***********  ��־����  *********
DEBUG  �����Լ��𣬵���ʱ�����һЩ������־������״̬���Թر�
INFO   �����������������״̬��־
WARNING����������������ľ�����־����������ܽ�һ�����³������Ҳ���ܶԳ�������û��Ӱ�죬���磺�Ǳ�Ҫ�������ɼ�����ʧ��
ERROR  ����������������Ĵ�����־���������������������ɽ�����⣬���磺��Ҫ�������ɼ�����ʧ��
*********************************/


/*********************  ��־��ʼ��  *************************/
// ��־���ͷ�ļ�
#include "easylogging++.h"

#include <string>
using std::string;

#define LOG_DEBUG_OUTPUT  1
#define LOG_INFO_OUTPUT   2
#define LOG_ERROR_OUTPUT  3

class MessageProcess
{
public:

private:

public:
	static string ProcessMessage(const CString msg, const unsigned int id);
	static string ProcessMessage(const CString msg);
	static string FormatLogMessage(const CString msg, const unsigned int id);
	static string FormatLogMessage(const CString msg);

	static void InitLogConfiguration();
	static CString GetFilePath(LPCTSTR const strCurFileName); // �����ļ����·����֯����·��
	static CString GetExePath(BOOL bIncludeSep); // false ����.exeִ���ļ�����Ŀ¼

};


#endif



