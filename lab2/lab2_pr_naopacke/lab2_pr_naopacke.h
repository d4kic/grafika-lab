
// lab2_pr_naopacke.h : main header file for the lab2_pr_naopacke application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Clab2prnaopackeApp:
// See lab2_pr_naopacke.cpp for the implementation of this class
//

class Clab2prnaopackeApp : public CWinApp
{
public:
	Clab2prnaopackeApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab2prnaopackeApp theApp;
