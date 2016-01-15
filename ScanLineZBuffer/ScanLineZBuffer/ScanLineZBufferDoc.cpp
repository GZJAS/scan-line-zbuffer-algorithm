
// ScanLineZBufferDoc.cpp : CScanLineZBufferDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ScanLineZBuffer.h"
#endif

#include "ScanLineZBufferDoc.h"
#include "ScanLineZBufferView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CScanLineZBufferDoc

IMPLEMENT_DYNCREATE(CScanLineZBufferDoc, CDocument)

BEGIN_MESSAGE_MAP(CScanLineZBufferDoc, CDocument)
	ON_COMMAND(ID_FILE_CLOSE, &CScanLineZBufferDoc::OnFileClose)
END_MESSAGE_MAP()


// CScanLineZBufferDoc ����/����

CScanLineZBufferDoc::CScanLineZBufferDoc()
{
	// TODO:  �ڴ����һ���Թ������

}

CScanLineZBufferDoc::~CScanLineZBufferDoc()
{
}

BOOL CScanLineZBufferDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	// ��һ�δ���ʾ����
	SetTitle(_T("��ǰ����ʾģ��"));
	return TRUE;
}




// CScanLineZBufferDoc ���л�

void CScanLineZBufferDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CScanLineZBufferDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CScanLineZBufferDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CScanLineZBufferDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CScanLineZBufferDoc ���

#ifdef _DEBUG
void CScanLineZBufferDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScanLineZBufferDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}



#endif //_DEBUG


// CScanLineZBufferDoc ����
bool CScanLineZBufferDoc::ReadMesh(const std::string filePath)
{
	m_mesh.clear();
	bool result = OpenMesh::IO::read_mesh(m_mesh, filePath);
	if (result) {
		// std::cout << logTime() << "ģ����Ϣ" << m_mesh.n_vertices() << " " << m_mesh.n_edges() << " " << m_mesh.n_faces() << std::endl;
		POSITION pos = GetFirstViewPosition();
		CScanLineZBufferView *pView = (CScanLineZBufferView*)GetNextView(pos);
		pView->InitMeshInfo(m_mesh);
		pView->CalcModelInfo();
		pView->FocusModel();
	}
	return result && m_mesh.n_vertices() > 0;
}

void CScanLineZBufferDoc::OnFileClose()
{
	POSITION pos = GetFirstViewPosition();
	CScanLineZBufferView *pView = (CScanLineZBufferView*)GetNextView(pos);
	
	if (m_mesh.n_vertices() <= 0)
	{
		MessageBox(NULL, _T("��ǰδ���κ��ļ����޷��رգ�"), _T("Error"), MB_OK);
		pView->DestoryMeshInfo();
		return;
	}
	if ((MessageBox(NULL, _T("ȷ�Ϲرյ�ǰģ���ļ���"), _T("�ļ��ر�"), MB_ICONWARNING | MB_OKCANCEL) == IDOK)) 
	{
		pView->DestoryMeshInfo();
		this->SetTitle(_T("��ǰ����ʾģ��"));
		this->m_mesh.clear();
		this->UpdateAllViews(0, 0, 0);
		return;
	}
}
