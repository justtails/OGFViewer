// ChildView.h : interface of the CChildView class
//


#pragma once
#include "OGFModel.h"

// CChildView window

class CChildView : public CWnd
{
	// Construction
public:
	CChildView();

	// Attributes
public:

	// Operations
public:

	// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation
public:
	virtual ~CChildView();
	BOOL InitRender();
	void RenderScene();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
	// Generated message map functions
protected:

	//{{AFX_MSG(CChildView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);

	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags,CPoint point);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void LoadModel();

	int			m_WindowWidth;
	int			m_WindowHeight;
	
	CPoint		m_OldMousePos;
	CPoint		m_NewMousePos;

	CPoint		m_OldMouseSPos;
	CPoint		m_NewMouseSPos;

	bool		m_MouseLPressed;
	bool		m_MouseRPressed;

	HINSTANCE	hInst;
	HDC			m_DContext;
	HGLRC		m_RContext;
	LPCSTR		pszFileName;
	OGFModel	*m_Model;
	std::string	m_PathToStalker;
};

