
// DownLoadKN.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDownLoadKNApp: 
// �йش����ʵ�֣������ DownLoadKN.cpp
//

class CDownLoadKNApp : public CWinApp
{
public:
	CDownLoadKNApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDownLoadKNApp theApp;