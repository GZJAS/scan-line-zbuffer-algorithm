
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "ScanLineZBuffer.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // ״ָ̬ʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// ��������̨�����е���
	//AllocConsole();
	//freopen("CONIN$", "r+t", stdin);
	//freopen("CONOUT$", "w+t", stdout);
}

CMainFrame::~CMainFrame()
{
	//FreeConsole();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.cx = 1024;
	cs.cy = 768;
	cs.style |= WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("����״̬��ʧ��!");
		return -1;
	}
	return 0;
}
