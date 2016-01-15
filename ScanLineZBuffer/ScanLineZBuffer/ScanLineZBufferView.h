
// ScanLineZBufferView.h : CScanLineZBufferView ��Ľӿ�
//

#pragma once

#include "ScanLineZBufferCore/ScanLineZBufferStructure.h"
#include "ScanLineZBufferCore/ScanLineZBufferCore.h"

class CScanLineZBufferView : public CView
{
protected: // �������л�����
	CScanLineZBufferView();
	DECLARE_DYNCREATE(CScanLineZBufferView)

// ����
public:
	CScanLineZBufferDoc* GetDocument() const;

// �Զ������
public:
	void InitMeshInfo(PolyMesh &mesh);
	void DestoryMeshInfo();
	void CalcModelInfo(); // ����ģ�͵�����λ�úͳߴ� 
	void FocusModel(); // ģ�ͷ����ӿ���
protected:
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
	BOOL InitializeGL();
	void SetBackgroundColor();
// �Զ������
private:
	int m_GLPixelIndex; // ���ظ�ʽ������ֵ 
	HGLRC m_hGLContext; // ���ƻ�����HGLRCָ��һ��rendering context���
	ScanLineZBufferCore *pZBufferCore; // ZBuffer�㷨����
	double m_z_delta; // ģ�������������ϵ�����
	Point m_centerPoint; // ģ�͵��м�λ��
	double m_modelScale; // ģ�ͳߴ�
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CScanLineZBufferView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // ScanLineZBufferView.cpp �еĵ��԰汾
inline CScanLineZBufferDoc* CScanLineZBufferView::GetDocument() const
   { return reinterpret_cast<CScanLineZBufferDoc*>(m_pDocument); }
#endif

