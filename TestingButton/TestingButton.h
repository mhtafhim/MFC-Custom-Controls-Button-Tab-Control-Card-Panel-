
// TestingButton.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestingButtonApp:
// See TestingButton.cpp for the implementation of this class
//

class CTestingButtonApp : public CWinApp
{
public:
	CTestingButtonApp();

// Overrides
public:
	virtual BOOL InitInstance();

	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestingButtonApp theApp;
