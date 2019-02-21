// OGFViewer.h : main header file for the OGFViewer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// COGFViewerApp:
// See OGFViewer.cpp for the implementation of this class
//

class COGFViewerApp : public CWinApp
{
public:
	COGFViewerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COGFViewerApp theApp;