
// ScanLineZBuffer.h : ScanLineZBuffer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CScanLineZBufferApp:
// �йش����ʵ�֣������ ScanLineZBuffer.cpp
//

class CScanLineZBufferApp : public CWinApp
{
public:
	CScanLineZBufferApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
	afx_msg void OnFileClose();
};

extern CScanLineZBufferApp theApp;
