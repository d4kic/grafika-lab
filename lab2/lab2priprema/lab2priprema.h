
// lab2priprema.h : main header file for the lab2priprema application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Clab2pripremaApp:
// See lab2priprema.cpp for the implementation of this class
//

class Clab2pripremaApp : public CWinApp
{
public:
	Clab2pripremaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab2pripremaApp theApp;
