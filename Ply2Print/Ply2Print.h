
// Ply2Print.h : Ply2Print Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CPly2PrintApp:
// �йش����ʵ�֣������ Ply2Print.cpp
//

class CPly2PrintApp : public CWinApp
{
public:
	CPly2PrintApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPly2PrintApp theApp;
