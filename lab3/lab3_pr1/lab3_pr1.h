
// lab3_pr1.h : main header file for the lab3_pr1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Clab3pr1App:
// See lab3_pr1.cpp for the implementation of this class
//

class Clab3pr1App : public CWinApp
{
public:
	Clab3pr1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab3pr1App theApp;
