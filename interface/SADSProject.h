
// SADSProject.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSADSProjectApp: 
// �йش����ʵ�֣������ SADSProject.cpp
//

class CSADSProjectApp : public CWinApp
{
public:
	CSADSProjectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSADSProjectApp theApp;

