
// lab3priprema.h : main header file for the lab3priprema application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Clab3pripremaApp:
// See lab3priprema.cpp for the implementation of this class
//

class Clab3pripremaApp : public CWinApp
{
public:
	Clab3pripremaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab3pripremaApp theApp;
