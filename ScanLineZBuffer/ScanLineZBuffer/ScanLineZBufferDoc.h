
// ScanLineZBufferDoc.h : CScanLineZBufferDoc ��Ľӿ�
//


#pragma once

#include "utility.h"

class CScanLineZBufferDoc : public CDocument
{
protected: // �������л�����
	CScanLineZBufferDoc();
	DECLARE_DYNCREATE(CScanLineZBufferDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CScanLineZBufferDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	// ����ģ���ļ�
	bool ReadMesh(const std::string filePath);

public:
	// ģ�ͱ���
	PolyMesh m_mesh;

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	afx_msg void OnFileClose();
};
