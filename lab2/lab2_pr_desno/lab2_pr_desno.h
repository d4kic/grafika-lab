
// lab2_pr_desno.h : main header file for the lab2_pr_desno application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Clab2prdesnoApp:
// See lab2_pr_desno.cpp for the implementation of this class
//

class Clab2prdesnoApp : public CWinApp
{
public:
	Clab2prdesnoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab2prdesnoApp theApp;
