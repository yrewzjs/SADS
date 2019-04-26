#ifndef _MESSAGEPROCESS_H_
#define _MESSAGEPROCESS_H_
/**
* 该类是一个消息处理类。
* @author zjs
* @Time 2017
*/

/***********  日志级别  *********
DEBUG  ：调试级别，调试时输出的一些调试日志，工作状态可以关闭
INFO   ：程序运行中输出的状态日志
WARNING：程序运行中输出的警告日志，该情况可能进一步导致程序错误，也可能对程序运行没有影响，比如：非必要传感器采集数据失败
ERROR  ：程序运行中输出的错误日志，该情况代表程序遭遇不可解决问题，比如：必要传感器采集数据失败
*********************************/


/*********************  日志初始化  *************************/
// 日志组件头文件
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
	static CString GetFilePath(LPCTSTR const strCurFileName); // 根据文件相对路径组织绝对路径
	static CString GetExePath(BOOL bIncludeSep); // false 返回.exe执行文件所在目录

};


#endif



