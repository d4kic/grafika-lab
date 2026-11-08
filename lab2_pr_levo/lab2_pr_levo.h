
// lab2_pr_levo.h : main header file for the lab2_pr_levo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Clab2prlevoApp:
// See lab2_pr_levo.cpp for the implementation of this class
//

class Clab2prlevoApp : public CWinApp
{
public:
	Clab2prlevoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab2prlevoApp theApp;
