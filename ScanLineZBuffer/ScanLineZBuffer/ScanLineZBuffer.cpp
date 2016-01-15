
// ScanLineZBuffer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ScanLineZBuffer.h"
#include "MainFrm.h"

#include "ScanLineZBufferDoc.h"
#include "ScanLineZBufferView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScanLineZBufferApp

BEGIN_MESSAGE_MAP(CScanLineZBufferApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CScanLineZBufferApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	// ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	// ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CScanLineZBufferApp::OnFileOpen)
END_MESSAGE_MAP()


// CScanLineZBufferApp ����

CScanLineZBufferApp::CScanLineZBufferApp()
{
	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ScanLineZBuffer.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CScanLineZBufferApp ����

CScanLineZBufferApp theApp;


// CScanLineZBufferApp ��ʼ��

BOOL CScanLineZBufferApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	// SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	// LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CScanLineZBufferDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CScanLineZBufferView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CScanLineZBufferApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileOpen();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_COMMAND(ID_FILE_OPEN, &CAboutDlg::OnFileOpen)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CScanLineZBufferApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CScanLineZBufferApp ��Ϣ�������





//void CAboutDlg::OnFileOpen()
//{
//	// TODO:  �ڴ���������������
//}


// ������ļ�
void CScanLineZBufferApp::OnFileOpen()
{
	// �ļ�����
	CString filter;
	filter = "OBJ files (*.obj)|*.obj|All Files (*.*)|*.*||";
	CFileDialog fileOpenDlg(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER, filter);
	if (fileOpenDlg.DoModal() == IDOK)
	{
		CScanLineZBufferDoc *pDoc = (CScanLineZBufferDoc*)AfxGetMainWnd()->GetTopLevelFrame()->GetActiveView()->GetDocument();
		std::string filePath = CT2A(fileOpenDlg.GetPathName().GetBuffer());
		CString fileName = fileOpenDlg.GetFileName();
		if (pDoc->ReadMesh(filePath))
		{
			MessageBox(NULL, _T("����objģ���ļ��ɹ�!"), _T("Success"), MB_OK);
			// �޸��ļ�����
			pDoc->SetTitle(fileName);
			// �����ػ���Ϣ
			pDoc->UpdateAllViews(0, 0, 0);
		} 
		else 
		{
			MessageBox(NULL, _T("����objģ���ļ�����!(������PolyMeshģ��)"), _T("Error"), MB_OK);
		}
	}

}
